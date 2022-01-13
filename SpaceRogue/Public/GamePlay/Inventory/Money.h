// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamePlay/Inventory/RogueAutomaticPickup.h"
#include "Money.generated.h"

/**
 * 
 */
UCLASS()
class SPACEROGUE_API AMoney : public ARogueAutomaticPickup
{
	GENERATED_BODY()

public:
	AMoney();
	
	void Collect_Implementation(APlayerController* Controller) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Value;
	
};
