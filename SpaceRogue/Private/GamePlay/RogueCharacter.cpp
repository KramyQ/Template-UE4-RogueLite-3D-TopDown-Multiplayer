// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlay/RogueCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/SphereComponent.h"
#include "GameCore/RoguePlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GamePlay/Inventory/RogueAutomaticPickup.h"
#include "GamePlay/Projectiles/RogueProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ARogueCharacter::ARogueCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	bUseControllerRotationRoll = false;
	// bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bReplicates = true;

	// GetCharacterMovement()->RotationRate = FRotator(0.0f,540.0f,0.0f);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->TargetArmLength = 1000.0f;
	CameraBoom->SetRelativeRotation(FRotator(-65.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	CursorToWorld->SetupAttachment(RootComponent);
	CursorToWorld->SetIsReplicated(true);
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/ProjectBP/M_Cursor_Decal.M_Cursor_Decal'"));
	if (DecalMaterialAsset.Succeeded())
	{
		CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}
	CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());

	// Create the collection sphere
	CollectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollectionSphere"));
	CollectionSphere->SetupAttachment(RootComponent);
	CollectionSphere->SetSphereRadius(200.f);
	
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	//Initialize projectile class
	ProjectileClass = ARogueProjectile::StaticClass();
	//Initialize fire rate
	FireRate = 0.25f;
	bIsFiringWeapon = false;
	
}

// Called when the game starts or when spawned
void ARogueCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	
}

// Called every frame
void ARogueCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AimAtMouse();
	CollectAutoPickups();
	CheckForInteractables();
	
}

// Called to bind functionality to input
void ARogueCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ARogueCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ARogueCharacter::MoveRight);
	// Handle firing projectiles
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ARogueCharacter::StartFire);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &ARogueCharacter::Dash);
	// PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ARogueCharacter::Interact);

}

void ARogueCharacter::MoveForward(float Axis)
{
	FVector DirectionX(1.0f,0,0);
	AddMovementInput(DirectionX*Axis*MoveSpeed);
}

void ARogueCharacter::MoveRight(float Axis)
{
	FVector DirectionY(0,1.0f,0);
	AddMovementInput(DirectionY*Axis*MoveSpeed);
}

void ARogueCharacter::AimAtMouse()
{	FVector cursorLocation;
	if (CursorToWorld != nullptr)
	{
		// Get mouse position on client
		if (ARoguePlayerController* PC = Cast<ARoguePlayerController>(GetController()))
		{
			FHitResult TraceHitResult;
			PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
			FVector CursorFV = TraceHitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();
			cursorLocation = TraceHitResult.Location;
		}
	}
	if(!HasAuthority())
	{
		///prediction on client owning current pawn only + //// Run rotation on Server
		if(IsLocallyControlled())
		{
			RotateCharAndSetCursorLocation(cursorLocation);
			Server_AimAtCursor(cursorLocation);
		}
	} else if (HasAuthority() & IsLocallyControlled())
	{
		RotateCharAndSetCursorLocation(cursorLocation);
	}
}

void ARogueCharacter::Server_AimAtCursor_Implementation(FVector Location)
{
	RotateCharAndSetCursorLocation(Location);
}

void ARogueCharacter::RotateCharAndSetCursorLocation(FVector Location)
{
	CursorToWorld->SetWorldLocation(Location);
	FRotator NewRotation(0,UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),Location).Yaw-GetActorRotation().Yaw,0);
	this->AddActorWorldRotation(NewRotation);
}

void ARogueCharacter::StartFire()
{
	if (!bIsFiringWeapon)
	{
		bIsFiringWeapon = true;
		UWorld* World = GetWorld();
		World->GetTimerManager().SetTimer(FiringTimer, this, &ARogueCharacter::StopFire, FireRate, false);
		HandleFire();
		// FVector spawnLocation = GetActorLocation() + ( GetActorRotation().Vector()  * 100.0f ) + (GetActorUpVector() * 50.0f);
		// FRotator spawnRotation = GetActorRotation();
		//
		// FActorSpawnParameters spawnParameters;
		// spawnParameters.Instigator = GetInstigator();
		// spawnParameters.Owner = this;
		// ARogueProjectile* spawnedProjectile = GetWorld()->SpawnActor<ARogueProjectile>(spawnLocation, spawnRotation, spawnParameters);
	}
}

void ARogueCharacter::StopFire()
{
	bIsFiringWeapon = false;
}

void ARogueCharacter::Dash()
{
	if(!HasAuthority())
	{
		///prediction on client owning current pawn only + //// Run rotation on Server
		if(IsLocallyControlled())
		{
			ApplyDash();
			Server_Dash();
			
		}
	} else if (HasAuthority() & IsLocallyControlled())
	{
		ApplyDash();
	}
}

void ARogueCharacter::ApplyDash()
{
	FVector DashVector = GetVelocity();
	DashVector.Normalize();
	DashVector = DashVector*5000;
	LaunchCharacter(DashVector,true,false);
}

void ARogueCharacter::ReceiveDamage(ARogueProjectile* projectile, const FHitResult& Hit, FVector NormalImpulse,
	AController* ProjectileInstigator, bool ApplyEffect)
{
	this->CurrentLifePoints -= projectile->Damage;
	UpdateHealthPoints();
	UE_LOG(LogTemp, Warning, TEXT("liefPoints:  %f"), this->CurrentLifePoints);
	if(CurrentLifePoints < 0)
	{
		Death();
		Destroy();
	}
}

void ARogueCharacter::Death()
{
	UE_LOG(LogTemp, Warning, TEXT("Death"));
}


void ARogueCharacter::Server_Dash_Implementation()
{
	ApplyDash();
}

void ARogueCharacter::HandleFire_Implementation()
{
	FVector spawnLocation = GetActorLocation() + ( GetActorRotation().Vector()  * 100.0f ) + (GetActorUpVector() * 50.0f);
	FRotator spawnRotation = GetActorRotation();

	FActorSpawnParameters spawnParameters;
	spawnParameters.Instigator = GetInstigator();
	spawnParameters.Owner = this;
	ARogueProjectile* spawnedProjectile = GetWorld()->SpawnActor<ARogueProjectile>(spawnLocation, spawnRotation, spawnParameters);
}

// INVENTORY MANAGEMENT

void ARogueCharacter::CollectAutoPickups()
{
	// Get all overlapping Actors and store them in an array
	TArray<AActor*> CollectedActors;
	CollectionSphere->GetOverlappingActors(CollectedActors);

	ARoguePlayerController* IController = Cast<ARoguePlayerController>(GetController());

	// For each collected Actor
	for (int32 iCollected = 0; iCollected < CollectedActors.Num(); ++iCollected)
	{
		// Cast the actor to AAutoPickup
		ARogueAutomaticPickup* const TestPickup = Cast<ARogueAutomaticPickup>(CollectedActors[iCollected]);
		// If the cast is successful and the pickup is valid and active 
		if (TestPickup && !TestPickup->IsPendingKill())
		{
			TestPickup->Collect(IController);
		}
	}
}

void ARogueCharacter::CheckForInteractables()
{
	// Create a LineTrace to check for a hit
	FHitResult HitResult;

	int32 Range = 500;
	FVector StartTrace = FollowCamera->GetComponentLocation();
	FVector EndTrace = (FollowCamera->GetForwardVector() * Range) + StartTrace;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	ARoguePlayerController* IController = Cast<ARoguePlayerController>(GetController());

	if (IController)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hello1"));
		// Check if something is hit
		FVector CursorPosition1 = CursorToWorld->GetComponentLocation();
		FVector CursorPosition2 = CursorPosition1;
		CursorPosition1.Z = CursorPosition1.Z+10;
		CursorPosition2.Z = CursorPosition1.Z-10;
		if (GetWorld()->LineTraceSingleByChannel(HitResult, CursorPosition1, CursorPosition2, ECC_Visibility, QueryParams))
		{
			// Cast the actor to AInteractable
			ARogueInteractable* Interactable = Cast<ARogueInteractable>(HitResult.GetActor());
			// If the cast is successful
			if (Interactable)
			{
				IController->CurrentInteractable = Interactable;
				return;
			}
		}

		IController->CurrentInteractable = nullptr;
	}
}

