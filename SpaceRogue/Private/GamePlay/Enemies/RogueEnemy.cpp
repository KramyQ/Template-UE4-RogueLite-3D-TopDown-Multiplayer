// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlay/Enemies/RogueEnemy.h"

#include "GamePlay/Projectiles/RogueProjectile.h"

// Sets default values
ARogueEnemy::ARogueEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	ProjectileClass = ARogueProjectile::StaticClass();
	//Initialize fire rate
	FireRate = 0.25f;
	FireLoopRate = 4.0f;
	bIsFiringWeapon = false;

}

// Called when the game starts or when spawned
void ARogueEnemy::BeginPlay()
{
	Super::BeginPlay();
	if(HasAuthority())
	{
		UWorld* World = GetWorld();
		World->GetTimerManager().SetTimer(FireLoopTimer, this, &ARogueEnemy::StartFire, FireLoopRate, true);
	}
	
	
}

// Called every frame
void ARogueEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARogueEnemy::StartFire()
{
	if (!bIsFiringWeapon)
	{
		bIsFiringWeapon = true;
		UWorld* World = GetWorld();
		World->GetTimerManager().SetTimer(FiringTimer, this, &ARogueEnemy::StopFire, FireRate, false);
		if(HasAuthority())
		{
			for (int i = 0; i < 8; i++) {
				FRotator spawnRotation(0,45*i,0);
				FVector spawnLocation = GetActorLocation() + ( spawnRotation.Vector()  * 100.0f ) + (GetActorUpVector() * 50.0f);
				FActorSpawnParameters spawnParameters;
				spawnParameters.Instigator = GetInstigator();
				spawnParameters.Owner = this;
				GetWorld()->SpawnActor<ARogueProjectile>(spawnLocation, spawnRotation, spawnParameters);
			}
		}
	}
}

void ARogueEnemy::StopFire()
{
	bIsFiringWeapon = false;
}

void ARogueEnemy::ReceiveDamage(ARogueProjectile* projectile, const FHitResult& Hit, FVector NormalImpulse,
	AController* ProjectileInstigator, bool ApplyEffect)
{
	this->CurrentLifePoints -= projectile->Damage;
	UpdateHealthPoints();
	UE_LOG(LogTemp, Warning, TEXT("liefPoints:  %f"), this->CurrentLifePoints);
	if(CurrentLifePoints < 0)
	{
		Death();
	}
	
}

void ARogueEnemy::Death()
{
	UE_LOG(LogTemp, Warning, TEXT("Death"));
	Destroy();
}

// void ARogueEnemy::HandleFire_Implementation()
// {
// 	FVector spawnLocation = GetActorLocation() + ( GetActorRotation().Vector()  * 100.0f ) + (GetActorUpVector() * 50.0f);
// 	for (int i = 0; i < 5; i++) {
// 		FActorSpawnParameters spawnParameters;
// 		spawnParameters.Instigator = GetInstigator();
// 		spawnParameters.Owner = this;
// 		ARogueProjectile* spawnedProjectile = GetWorld()->SpawnActor<ARogueProjectile>(spawnLocation, spawnRotation, spawnParameters);
// 	}
// 	FRotator spawnRotation = GetActorRotation();
// 	
// }

