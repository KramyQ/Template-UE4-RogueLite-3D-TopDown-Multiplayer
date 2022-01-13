// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlay/Inventory/RogueInteractable.h"

// Sets default values
ARogueInteractable::ARogueInteractable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Name = "Interactable";
	Action = "interact";

}

void ARogueInteractable::Interact_Implementation(APlayerController* Controller)
{
	return;
}

FString ARogueInteractable::GetInteractText() const 
{
	return FString::Printf(TEXT("%s: Press E to %s"), *Name, *Action); 
}


