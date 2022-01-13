// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamePlay/Inventory/RogueInteractable.h"
#include "RogueManualPickup.generated.h"

/**
 * 
 */
UCLASS()
class SPACEROGUE_API ARogueManualPickup : public ARogueInteractable
{
	GENERATED_BODY()
public:
	ARogueManualPickup();

	void Interact_Implementation(APlayerController* Controller) override;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemManagement")
	UStaticMeshComponent* PickupMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemManagement")
	int32 ItemID;
};

