#pragma once
#include "Structs.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	WEAPON,
	MOD,
	EQUIPEMENT
};

USTRUCT(BlueprintType)
struct FRogueItemData
{
	GENERATED_BODY()

	UPROPERTY(Category = CharacterStats, EditAnywhere)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 ItemID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EItemType ItemType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<class ARogueItem> ItemActor;

	UPROPERTY(BlueprintReadWrite)
	class UTexture2D* ItemIcon;

	

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float ItemCost;
};