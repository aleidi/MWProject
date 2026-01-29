#pragma once

// Include Header
#include "UObject/NoExportTypes.h"
#include "MWSaveGameManager.generated.h"

// Forward Declare
class UMWPlayerSaveGame;

// Define
#define SAVEGAMEMANAGER(WorldContext) (UMWSaveGameManager::Get(WorldContext))

/*
 * @class UMWSaveGameManager
 * 
 * @brief 
 *
 * @note
 */
UCLASS()
class UMWSaveGameManager : public UObject
{
	GENERATED_BODY()
	
public:
	void Initialize();
	void Deinitialize();

	UFUNCTION(BlueprintPure, Category = "Manager", meta = (WorldContext="WorldContext", DisplayName="GetSaveGameManager"))
	static UMWSaveGameManager* Get(const UObject* WorldContext);

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void SaveGameToSlot(int32 SlotIndex);

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void LoadGameFromSlot(int32 SlotIndex);

	bool DoesSaveExist(int32 SlotIndex) const;

	bool DeleteSaveSlot(int32 SlotIndex);

	UFUNCTION(BlueprintPure, Category = "SaveGame")
	UMWPlayerSaveGame* GetPlayerSaveGame() const;

private:
	UPROPERTY()
	TObjectPtr<UMWPlayerSaveGame> PlayerSaveGame;
};