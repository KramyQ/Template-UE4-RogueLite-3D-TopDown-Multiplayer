// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamePlay/Inventory/RogueInteractable.h"
#include "RogueAutomaticPickup.generated.h"

/**
 * 
 */
UCLASS()
class SPACEROGUE_API ARogueAutomaticPickup : public ARogueInteractable
{
	GENERATED_BODY()

public:
	ARogueAutomaticPickup();
	
	UFUNCTION(BlueprintNativeEvent)
	void Collect(APlayerController* Controller);
	virtual void Collect_Implementation(APlayerController* Controller);

	int32 GetItemID();

protected:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* PickupMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ItemID;
	
};
