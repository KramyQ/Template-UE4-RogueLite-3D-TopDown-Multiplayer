// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlay/Inventory/RogueInventory.h"

URogueInventory::URogueInventory()
{
	for (int32 i = 0; i < MaxInventorySlots; i++)
	{
		FName CurrentSlotName = FName(*FString::FromInt(i));
		URogueItemSlot * CurrentItemSlot = CreateDefaultSubobject<URogueItemSlot>(CurrentSlotName);
		CurrentItemSlot->Position = i;
		Inventory.Add(CurrentItemSlot);
	}

	for (int32 i = 0; i < 3; i++)
	{
		FString weaponName = "Weapon";
		FName CurrentSlotName = FName(*FString::FromInt(i).Append("Weapon"));
		URogueItemSlot * CurrentItemSlot = CreateDefaultSubobject<URogueItemSlot>(CurrentSlotName);
		CurrentItemSlot->Position = i;
		WeaponSlots.Add(CurrentItemSlot);
	}
}

bool URogueInventory::AddItem(FRogueItemData NewItemData)
{
	if(CurrentInventoryOccupation == MaxInventorySlots)
	{
		return false;
	}
	for (int32 i = 0; i < MaxInventorySlots; i++)
	{
		if(!Inventory[i]->GetOccupied())
		{
			Inventory[i]->SetItemData(NewItemData);
			Inventory[i]->SetOccupied(true);
			return true;
		}
	}
	return false;
}

FRogueItemData URogueInventory::GetItem(int32 ItemPosition)
{
	if(ItemPosition < MaxInventorySlots)
	{
		return Inventory[ItemPosition]->GetItemData();
	}
	FRogueItemData empty;
	return empty;
}

bool URogueInventory::DeleteItem(int32 ItemPosition)
{
	if(ItemPosition < MaxInventorySlots)
	{
		Inventory.RemoveAt(ItemPosition);
		return true;
	}
	return false;
}

bool URogueInventory::MoveItem(int32 InitialItemPosition, int32 FinalItemPosition)
{
	if(InitialItemPosition != FinalItemPosition && InitialItemPosition < MaxInventorySlots && FinalItemPosition < MaxInventorySlots){
		FRogueItemData tempData;
		bool tempBool;
		tempData = Inventory[InitialItemPosition]->GetItemData();
		tempBool = Inventory[InitialItemPosition]->GetOccupied();
		Inventory[InitialItemPosition]->SetOccupied(Inventory[FinalItemPosition]->GetOccupied());
		Inventory[FinalItemPosition]->SetOccupied(tempBool);
		Inventory[InitialItemPosition]->SetItemData(Inventory[FinalItemPosition]->GetItemData());
		Inventory[FinalItemPosition]->SetItemData(tempData);
		return true;
	}
	return false;
	
}
