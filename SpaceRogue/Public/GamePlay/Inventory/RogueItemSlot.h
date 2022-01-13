// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Structs.h"
#include "UObject/NoExportTypes.h"
#include "RogueItemSlot.generated.h"

/**
 * 
 */
UCLASS()
class SPACEROGUE_API URogueItemSlot : public UObject
{
	GENERATED_BODY()
public:
	int32 Position;

	
	int32 GetPosition() const
	{
		return Position;
	}
	
	UFUNCTION(BlueprintCallable, Category = "ItemManagement")
	bool GetOccupied() const
	{
		return Occupied;
	}

	void SetOccupied(bool bOccupied)
	{
		Occupied = bOccupied;
	}
	
	UFUNCTION(BlueprintCallable, Category = "ItemManagement")
	FRogueItemData GetItemData() const
	{
		return ItemData;
	}

	void SetItemData(const FRogueItemData& NewItemData)
	{
		this->ItemData = NewItemData;
	}

	bool Occupied = false;
	FRogueItemData ItemData;

	
	
};
