#pragma once

// Include Header
#include "UObject/NoExportTypes.h"
#include "Define/MWDefineGameplay.h"
#include "MWPartyManager.generated.h"

// Forward Declare

// Define

/*
 * @class UMWPartyManager
 * 
 * @brief The manager of party and teams.
 *
 * @note
 */
UCLASS()
class MW_API UMWPartyManager : public UObject, public IMWManagerInterface
{
	GENERATED_BODY()

public:
	void Initialize() {}
	void Deinitialize() {}

	UFUNCTION(BlueprintPure, Category = "Manager", meta = (WorldContext="WorldContext", DisplayName = "GetPartyManager"))
	static UMWPartyManager* Get(const UObject* WorldContext);

private:
	UPROPERTY()
    TMap<int32, FMWCharacterRuntimeData> CharacterDataCache;
};