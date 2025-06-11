#pragma once

#include "Engine/DataAsset.h"
#include "Data/MWCharacterData.h"
#include "MWMasterData.generated.h"

// Forward Declare
class AMWCharacter;
class UMWInputConfig;
class UMWUIConfigData;

/*
* UMWMasterData
* 
* Data assets is necessary to run the game.
*/
UCLASS()
class UMWMasterData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Default")
	TSoftClassPtr<AMWCharacter> DefaultPawn;

	/* FMWCharacterMasterData : Main character data  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character")
	TObjectPtr<UDataTable> MainCharacterData;

	// Input configuration used by player controlled pawns to create input mappings and bind input actions.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UMWInputConfig> InputConfig;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UMWUIConfigData> UIConfig;
};