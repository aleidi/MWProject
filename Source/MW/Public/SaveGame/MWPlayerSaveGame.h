#pragma once

// Include Header
#include "Define/MWDefineGameplay.h"
#include "GameFramework/SaveGame.h"
#include "MWPlayerSaveGame.generated.h"

// Forward Declare

// Define

/*
 * @class UMWPlayerSaveGame
 * 
 * @brief 
 *
 * @note
 */
UCLASS()
class UMWPlayerSaveGame : public USaveGame
{
	GENERATED_BODY()
	
	// Function to handle save data
public:
	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void ChangeData();

	// Data of member
private:
	UPROPERTY(BlueprintReadWrite, Category = "SaveGame", meta = (AllowPrivateAccess = "true"))
	FMWPartyData PartyData;
};