// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlay/Inventory/RogueManualPickup.h"

#include "GameCore/RoguePlayerController.h"

ARogueManualPickup::ARogueManualPickup()
{
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>("PickupMesh");
	RootComponent = Cast<USceneComponent>(PickupMesh);

	ItemID = 0;

	Super::Name = "Item";
	Super::Action = "pickup";
}

void ARogueManualPickup::Interact_Implementation(APlayerController* Controller)
{
	Super::Interact_Implementation(Controller);

	ARoguePlayerController* IController = Cast<ARoguePlayerController>(Controller);
	if(IController->AddItemToInventoryByID(ItemID))
		Destroy();
}
