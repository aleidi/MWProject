#pragma once

// Include Header
#include "Interface/MWManagerInterface.h"
#include "UObject/NoExportTypes.h"
#include "MWCharacterDataManager.generated.h"

// Forward Declare
struct FMWCharacterDataRow;

// Define
#define GET_CHARDATAMGR(WorldContext)  (UMWCharacterDataManager::Get(WorldContext))

/*
 * @class UMWCharacterDataManager
 * 
 * @brief 
 *
 * @note
 */
UCLASS()
class UMWCharacterDataManager : public UObject, public IMWManagerInterface
{
	GENERATED_BODY()
	
public:
	UMWCharacterDataManager();

	virtual void Initialize() override;
	virtual void Deinitialize() override {}

	UFUNCTION(BlueprintPure, Category="Manager", meta=(WorldContext="WorldContext", DisplayName="GetGameDataManager"))
	static UMWCharacterDataManager* Get(const UObject* WorldContext);
	
	FMWCharacterDataRow* GetCharacterData(int32 CharacterId) const;

	UFUNCTION(BlueprintCallable, Category="Manager|Character", meta=(DisplayName="GetCharacterData"))
	bool K2_GetCharacterData(int32 CharacterId, FMWCharacterDataRow& OutData) const;

private:
	bool LoadCharacterData();

private:
	UPROPERTY()
	TObjectPtr<UDataTable> CharacterData;

private:
	static FString DataPath;

	static FString CharacterDataPath;
};