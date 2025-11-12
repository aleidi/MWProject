#pragma once

#include "Engine/DataAsset.h"
#include "MWMasterData.generated.h"

// Forward Declare
class AMWCharacter;
class UMWInputConfig;
class UMWUIConfigData;
class UMWGameplayData;
class UC3DCameraComponent;
class UC3DCameraModeDataAsset;

/*
* UMWMasterData
* 
* Data assets that are necessary to run the game.
*/
UCLASS()
class UMWMasterData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Default")
	TSoftClassPtr<AMWCharacter> DefaultPawn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Default")
	TSubclassOf<UAnimInstance> DummyAnimClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Default")
	TObjectPtr<USkeletalMesh> DummyMesh;

	// Input configuration used by player controlled pawns to create input mappings and bind input actions.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UMWInputConfig> InputConfig;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UMWUIConfigData> UIConfig;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
	TArray<TObjectPtr<UC3DCameraModeDataAsset>> DefaultCameraModesAssets;
};