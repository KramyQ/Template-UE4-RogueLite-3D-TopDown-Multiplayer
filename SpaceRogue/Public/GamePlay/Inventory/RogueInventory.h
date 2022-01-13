// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RogueItemSlot.h"
#include "UObject/NoExportTypes.h"
#include "RogueInventory.generated.h"

/**
 * 
 */
UCLASS()
class SPACEROGUE_API URogueInventory : public UObject
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	URogueInventory();
	UPROPERTY(BlueprintReadOnly)
	TArray<URogueItemSlot*> Inventory;

	UPROPERTY(BlueprintReadOnly)
	TArray<URogueItemSlot*> WeaponSlots;
	
	UFUNCTION(BlueprintCallable, Category = "ItemManagement")
	TArray<URogueItemSlot*> GetInventory() const
	{
		return Inventory;
	}

	int32 MaxInventorySlots = 20;
	UPROPERTY()
	int32 CurrentInventoryOccupation = 0;

	UFUNCTION(BlueprintCallable, Category = "ItemManagement")
	bool AddItem(FRogueItemData NewItemData);

	UFUNCTION(BlueprintCallable, Category = "ItemManagement")
	FRogueItemData GetItem(int32 ItemPosition);
	
	UFUNCTION(BlueprintCallable, Category = "ItemManagement")
	bool DeleteItem(int32 ItemPosition);
	
	UFUNCTION(BlueprintCallable, Category = "ItemManagement")
	bool MoveItem(int32 InitialItemPosition, int32 FinalItemPosition);

	

	
	
	
};
