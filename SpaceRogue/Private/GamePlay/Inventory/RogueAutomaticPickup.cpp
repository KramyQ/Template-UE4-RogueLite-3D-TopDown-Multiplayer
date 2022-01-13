// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlay/Inventory/RogueAutomaticPickup.h"

#include "GameCore/RoguePlayerController.h"

ARogueAutomaticPickup::ARogueAutomaticPickup()
{
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>("PickupMesh");
	RootComponent = Cast<USceneComponent>(PickupMesh);

	ItemID = 0;
}


void ARogueAutomaticPickup::Collect_Implementation(APlayerController* Controller)
{
	ARoguePlayerController* IController = Cast<ARoguePlayerController>(Controller);
	if(IController->AddItemToInventoryByID(ItemID))
		Destroy();
}

int32 ARogueAutomaticPickup::GetItemID()
{
	return ItemID;
}
