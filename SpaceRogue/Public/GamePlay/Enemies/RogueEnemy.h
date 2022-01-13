// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RogueEnemy.generated.h"

UCLASS()
class SPACEROGUE_API ARogueEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARogueEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(Category = CharacterStats, EditAnywhere)
	float MoveSpeed = 1;
	
	UPROPERTY(Category = CharacterStats, EditAnywhere)
	float CurrentLifePoints = 100;

	UPROPERTY(Category = CharacterStats, EditAnywhere)
	float MaxLifePoints = 100;

	UPROPERTY(Category = CharacterStats, EditAnywhere)
	float Defense = 0;

	UPROPERTY(Category = CharacterStats, EditAnywhere)
	float CriticalScore = 1;

	UPROPERTY(Category = CharacterStats, EditAnywhere)
	float AttackSpeedScore = 1;

	UPROPERTY(Category = CharacterStats, EditAnywhere)
	float AttackPowerScore = 1;

	UPROPERTY(EditDefaultsOnly, Category="Gameplay|Projectile")
	TSubclassOf<class ARogueProjectile> ProjectileClass;

	/** Delay between shots in seconds. Used to control fire rate for our test projectile, but also to prevent an overflow of server functions from binding SpawnProjectile directly to input.*/
	UPROPERTY(EditDefaultsOnly, Category="Gameplay")
	float FireRate;
	float FireLoopRate;

	/** If true, we are in the process of firing projectiles. */
	bool bIsFiringWeapon;

	/** Function for beginning weapon fire.*/
	UFUNCTION(BlueprintCallable, Category="Gameplay")
	void StartFire();

	/** Function for ending weapon fire. Once this is called, the player can use StartFire again.*/
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void StopFire();  
	

	/** A timer handle used for providing the fire rate delay in-between spawns.*/
	FTimerHandle FiringTimer;
	FTimerHandle FireLoopTimer;

	void ReceiveDamage(ARogueProjectile* projectile,const FHitResult& Hit,FVector NormalImpulse, AController* ProjectileInstigator, bool ApplyEffect);

	UFUNCTION(BlueprintImplementableEvent, Category = "Gameplay")
	void UpdateHealthPoints();

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void Death();
};
