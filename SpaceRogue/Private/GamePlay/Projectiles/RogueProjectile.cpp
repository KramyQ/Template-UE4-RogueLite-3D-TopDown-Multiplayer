// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlay/Projectiles/RogueProjectile.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/DamageType.h"
#include "GamePlay/RogueCharacter.h"
#include "GamePlay/Enemies/RogueEnemy.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
// Sets default values
ARogueProjectile::ARogueProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	SphereComponent->InitSphereRadius(37.5f);
	SphereComponent->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	RootComponent = SphereComponent;

	if (GetLocalRole() == ROLE_Authority)
	{
		SphereComponent->OnComponentHit.AddDynamic(this, &ARogueProjectile::OnProjectileImpact);
	}
	
	//Definition for the Mesh that will serve as our visual representation.
	static ConstructorHelpers::FObjectFinder<UStaticMesh> DefaultMesh(TEXT("/Game/ProjectBP/GamePlay/Projectiles/Sphere1"));
	// static ConstructorHelpers::FObjectFinder<UParticleSystem> DefaultParticle(TEXT("/Game/GabrielAguiarProd/ToonProjectilesVol1/Particles/Projectiles/P_Projectile_Bomb03Blue"));
	ProjectileVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	ProjectileVisual->SetupAttachment(RootComponent);
	//Set the Static Mesh and its position/scale if we successfully found a mesh asset to use.
	if (DefaultMesh.Succeeded())
	{
		ProjectileVisual->SetStaticMesh(DefaultMesh.Object);
		ProjectileVisual->SetRelativeLocation(FVector(0.0f, 0.0f, -37.5f));
		ProjectileVisual->SetRelativeScale3D(FVector(0.75f, 0.75f, 0.75f));
		
		// UGameplayStatics::SpawnEmitterAttached(
		// DefaultParticle.Object, //UParticleSystem*  
		// ProjectileVisual, 
		// FName("projectile"), 
		// FVector(-10,0,0), 
		// FRotator(0,90,0), 
		// EAttachLocation::KeepRelativeOffset, 
		// true
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> DefaultExplosionEffect(TEXT("/Game/GabrielAguiarProd/ToonProjectilesVol1/Particles/Hits/P_Hit_Bullet01Blue"));
	if (DefaultExplosionEffect.Succeeded())
	{
		ExplosionEffect = DefaultExplosionEffect.Object;
	}
	
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovementComponent->SetUpdatedComponent(SphereComponent);
	ProjectileMovementComponent->InitialSpeed = 1500.0f;
	ProjectileMovementComponent->MaxSpeed = 1500.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	
	DamageType = UDamageType::StaticClass();
	Damage = 10.0f;


	// Hides visible components
	// if(GetOwner() != GetNetOwner())
	// {
	// 	SetActorHiddenInGame(true);
	//
	// 	// Disables collision components
	// 	SetActorEnableCollision(false);
	//
	// 	// Stops the Actor from ticking
	// 	SetActorTickEnabled(false);
	// 	
	// }

}

// Called when the game starts or when spawned
void ARogueProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	
}

// Called every frame
void ARogueProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARogueProjectile::Destroyed()
{
	FVector spawnLocation = GetActorLocation();
	UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionEffect, spawnLocation, FRotator::ZeroRotator, true, EPSCPoolMethod::AutoRelease);
}

void ARogueProjectile::OnProjectileImpact(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if(HasAuthority())
	{
		if (OtherActor && OtherActor->IsA(ARogueCharacter::StaticClass()))
		{
			Cast<ARogueCharacter>(OtherActor)->ReceiveDamage(this, Hit, NormalImpulse, GetInstigator()->Controller, false);
		}
		if(OtherActor && OtherActor->IsA(ARogueEnemy::StaticClass()))
		{
			Cast<ARogueEnemy>(OtherActor)->ReceiveDamage(this, Hit, NormalImpulse, GetInstigator()->Controller, false);
		}
	}
	Destroy();
}



