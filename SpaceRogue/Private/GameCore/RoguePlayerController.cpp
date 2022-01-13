// Fill out your copyright notice in the Description page of Project Settings.


#include "GameCore/RoguePlayerController.h"

#include "Engine/DataTable.h"
#include "GameCore/RogueGameState.h"

ARoguePlayerController::ARoguePlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
	PlayerInventory = CreateDefaultSubobject<URogueInventory>(TEXT("PlayerInventory"));
}


void ARoguePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("Interact", IE_Pressed, this, &ARoguePlayerController::Interact);
}

bool ARoguePlayerController::AddItemToInventoryByID(int32 ID)
{
	ARogueGameState* GameState = Cast<ARogueGameState>(GetWorld()->GetGameState());
	FRogueItemData ItemToAdd;
	if(GameState->ItemList->Data.Num() > ID)
	{
		ItemToAdd = GameState->ItemList->Data[ID];
		if (PlayerInventory->CurrentInventoryOccupation < PlayerInventory->MaxInventorySlots)
		{
			PlayerInventory->AddItem(ItemToAdd);
			ReloadInventory();
			return true;
		}
	}
	return false;
}

void ARoguePlayerController::Interact()
{
	if (CurrentInteractable)
	{
		CurrentInteractable->Interact(this);
	}
}



