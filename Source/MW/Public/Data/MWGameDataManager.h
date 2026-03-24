#pragma once

// Include Header
#include "Data/MWGameDataTypes.h"
#include "Interface/MWManagerInterface.h"
#include "UObject/NoExportTypes.h"
#include "MWGameDataManager.generated.h"

// Forward Declare

// Define

/*
 * @class UMWGameDataManager
 * 
 * @brief 
 *
 * @note
 */
UCLASS()
class UMWGameDataManager : public UObject, public IMWManagerInterface
{
	GENERATED_BODY()
	
public:
	UMWGameDataManager();

	virtual void Initialize() override;
	virtual void Deinitialize() override {}

	UFUNCTION(BlueprintPure, Category="Manager", meta=(WorldContext="WorldContext", DisplayName="GetGameDataManager"))
	static UMWGameDataManager* Get(const UObject* WorldContext);

	
	FMWCharacterRegistryRow* GetCharacterData(int32 CharacterId) const;

	UFUNCTION(BlueprintCallable, Category="Manager|Character", meta=(DisplayName="GetCharacterData"))
	bool K2_GetCharacterData(int32 CharacterId, FMWCharacterRegistryRow& OutData) const;

private:
	void LoadAllData();

	bool LoadCharacterData();

private:
	UPROPERTY()
	TObjectPtr<UDataTable> CharacterData;

private:
	FString DataPath = TEXT("");

	FString CharacterDataPath = TEXT("");
};