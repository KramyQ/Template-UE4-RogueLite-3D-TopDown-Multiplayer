// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlay/Inventory/Money.h"

#include "GameCore/RoguePlayerController.h"

AMoney::AMoney()
{
	Super::ItemID = 1000;
	Value = 1;
}


void AMoney::Collect_Implementation(APlayerController* Controller)
{
	ARoguePlayerController	* IController = Cast<ARoguePlayerController>(Controller);
	IController->Money += Value;
	Destroy();
}
