// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RogueInteractable.generated.h"

UCLASS()
class SPACEROGUE_API ARogueInteractable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARogueInteractable();
	
	UFUNCTION(BlueprintNativeEvent)
	void Interact(APlayerController* Controller);
	virtual void Interact_Implementation(APlayerController* Controller);

	UPROPERTY(EditAnywhere)
	FString Name;

	UPROPERTY(EditAnywhere)
	FString Action;

	UFUNCTION(BlueprintCallable, Category = "Pickup")
	FString GetInteractText() const;

};
