#pragma once

#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
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

	/**
	 * Map of all input configurations, keyed by IMC GameplayTag.
	 * Use FindInputConfig() to retrieve a specific config at runtime.
	 *
	 * Recommended tag naming convention:
	 *   IMC.TPDefault, IMC.Basic, IMC.BattleCommand, IMC.BattleCommand.Attack, etc.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TMap<FGameplayTag, TObjectPtr<UMWInputConfig>> InputConfigs;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UMWUIConfigData> UIConfig;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
	TArray<TObjectPtr<UC3DCameraModeDataAsset>> DefaultCameraModesAssets;

public:
	/**
	 * Find an InputConfig by its IMC GameplayTag.
	 * @return nullptr if not found.
	 */
	UFUNCTION(BlueprintPure, Category = "Input")
	UMWInputConfig* FindInputConfig(const FGameplayTag& IMCTag) const;
};