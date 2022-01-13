// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Structs.h"
#include "EnvironmentQuery/EnvQueryDebugHelpers.h"
#include "GameFramework/Character.h"
#include "RogueCharacter.generated.h"


UCLASS()
class SPACEROGUE_API ARogueCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARogueCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	void AimAtMouse();
	void RotateCharAndSetCursorLocation(FVector Location);
	
	UFUNCTION(Server, Reliable)
	void Server_AimAtCursor(FVector Location);
	void Server_AimAtCursor_Implementation(FVector Location);

	/** Collection sphere */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* CollectionSphere;
	
	UPROPERTY(Category = CharacterStats, EditAnywhere)
	float MoveSpeed = 1;
	
	UPROPERTY(Category = CharacterStats, EditAnywhere, BlueprintReadWrite)
	float CurrentLifePoints = 100;

	UPROPERTY(Category = CharacterStats, EditAnywhere, BlueprintReadWrite)
	float MaxLifePoints = 100;

	UPROPERTY(Category = CharacterStats, EditAnywhere)
	float Defense = 0;

	UPROPERTY(Category = CharacterStats, EditAnywhere)
	float CriticalScore = 1;

	UPROPERTY(Category = CharacterStats, EditAnywhere)
	float AttackSpeedScore = 1;

	UPROPERTY(Category = CharacterStats, EditAnywhere)
	float AttackPowerScore = 1;
	
	

	/** The camera */
	UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;


	void MoveForward(float Axis);
	void MoveRight(float Axis);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    	class UDecalComponent* CursorToWorld;

	UPROPERTY(EditDefaultsOnly, Category="Gameplay|Projectile")
	TSubclassOf<class ARogueProjectile> ProjectileClass;

	/** Delay between shots in seconds. Used to control fire rate for our test projectile, but also to prevent an overflow of server functions from binding SpawnProjectile directly to input.*/
	UPROPERTY(EditDefaultsOnly, Category="Gameplay")
	float FireRate;

	/** If true, we are in the process of firing projectiles. */
	bool bIsFiringWeapon;

	/** Function for beginning weapon fire.*/
	UFUNCTION(BlueprintCallable, Category="Gameplay")
	void StartFire();

	/** Function for ending weapon fire. Once this is called, the player can use StartFire again.*/
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void StopFire();  

	/** Server function for spawning projectiles.*/
	UFUNCTION(Server, Reliable)
	void HandleFire();

	/** A timer handle used for providing the fire rate delay in-between spawns.*/
	FTimerHandle FiringTimer;
	void Dash();
	void ApplyDash();
	UFUNCTION(Server, Reliable)
	void Server_Dash();

	void ReceiveDamage(ARogueProjectile* projectile,const FHitResult& Hit,FVector NormalImpulse, AController* ProjectileInstigator, bool ApplyEffect);

	UFUNCTION(BlueprintImplementableEvent, Category = "Gameplay")
	void UpdateHealthPoints();

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void Death();

	// UFUNCTION(BlueprintImplementableEvent, Category = "Gameplay")
	// void AddItemToInventoryWidget(FItemData ItemData);
	
	/** Function to collect every AutoPickup in range. */
	void CollectAutoPickups();

	/** Function to check for the closest Interactable in sight and in range. */
	void CheckForInteractables();
	
};

