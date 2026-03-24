#pragma once

// Include Header
#include "Define/MWDefineGameplay.h"
#include "GameFramework/SaveGame.h"
#include "MWSaveGame.generated.h"

// Forward Declare

// Define
USTRUCT(BlueprintType)
struct FMWSaveMetaData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "SaveGame")
	int32 SaveId;

	UPROPERTY(BlueprintReadWrite, Category = "SaveGame")
	FString Slot;

	UPROPERTY(BlueprintReadWrite, Category = "SaveGame")
	int32 SaveVersion;

	UPROPERTY(BlueprintReadWrite, Category = "SaveGame")
	float PlayTime = 0.f;

	UPROPERTY(BlueprintReadWrite, Category = "SaveGame")
	FDateTime SaveDateTime;
};

USTRUCT(BlueprintType)
struct FMWCharacterSaveData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "SaveGame")
	int32 CharacterId;

	UPROPERTY(BlueprintReadWrite, Category = "SaveGame")
	int32 Level;

	UPROPERTY(BlueprintReadWrite, Category = "SaveGame")
	int32 Experience;

	UPROPERTY(BlueprintReadWrite, Category = "SaveGame")
	TArray<int32> LearnedSkills;

	// Equipped skills with their respective slot index
	UPROPERTY(BlueprintReadWrite, Category = "SaveGame")
	TMap<int32, int32> EquippedSkills;
};

USTRUCT(BlueprintType)
struct FMWPartyData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "SaveGame")
	int32 LeaderCharacterId;

	UPROPERTY(BlueprintReadWrite, Category = "SaveGame")
	TArray<FMWCharacterSaveData> PartyMembers;
};

/*
 * @class UMWSaveGame
 * 
 * @brief 
 *
 * @note
 */
UCLASS()
class UMWSaveGame : public USaveGame
{
	GENERATED_BODY()
	
	// Function to handle save data
public:
	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void ChangeData();

public:
	UPROPERTY(BlueprintReadWrite, Category = "SaveGame")
	FMWSaveMetaData MetaData;

	UPROPERTY(BlueprintReadWrite, Category = "SaveGame")
	FMWPartyData PartyData;

	//QUEST
	//Map-Region
	//Inventory
};