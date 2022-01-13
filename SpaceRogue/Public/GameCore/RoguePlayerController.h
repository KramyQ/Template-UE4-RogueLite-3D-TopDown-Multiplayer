// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GamePlay/Inventory/RogueInteractable.h"
#include "GamePlay/Inventory/RogueInventory.h"
#include "RoguePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SPACEROGUE_API ARoguePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ARoguePlayerController();

	UFUNCTION(BlueprintImplementableEvent)
	void ReloadInventory();
	

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 Money = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class ARogueInteractable * CurrentInteractable;
	
	UPROPERTY(BlueprintReadOnly, Category = "InventorySystem")
	URogueInventory * PlayerInventory;
	
	UFUNCTION(BlueprintCallable, Category = "Utils")
	URogueInventory* GetPlayerInventory()
	{
		return PlayerInventory;
	}

	UFUNCTION(BlueprintCallable, Category = "Utils")
	bool AddItemToInventoryByID(int32 ID);

protected:
	void Interact();
	
	virtual void SetupInputComponent() override;
};