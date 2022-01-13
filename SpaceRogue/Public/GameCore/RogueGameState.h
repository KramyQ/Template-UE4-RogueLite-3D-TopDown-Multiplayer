// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "GamePlay/Inventory/ItemData.h"
#include "RogueGameState.generated.h"
/**
 * 
 */
UCLASS()
class SPACEROGUE_API ARogueGameState : public AGameStateBase
{
	GENERATED_BODY()


public:
	ARogueGameState();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UItemData * ItemList;

	
	
};
