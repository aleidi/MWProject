#pragma once

#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "MWGameplayData.generated.h"

// Forward Declare
class AMWCharacter;
class UC3DCameraComponent;
class UC3DCameraModeDataAsset;
class UGameplayEffect;
class UMWInputConfig;
class UMWGameplayData;
class UMWUIConfigData;
/*
 * @class UMWGameplayData
 *
 * @brief Gameplay related const data.
 *        ゲームプレイに関連する定数データ。
 *
 */
UCLASS(BlueprintType, Const, Meta = (DisplayName = "MW Gameplay Data", ShortTooltip = "Data asset containing gaameplay data."))
class UMWGameplayData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Default")
	TSubclassOf<AMWCharacter> DefaultPawn;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Default")
	//TObjectPtr<UMWCharacterData> DefaultPawnData;

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

public:
	// Gameplay effect used to add and remove dynamic tags.
	UPROPERTY(EditDefaultsOnly, Category = "Battle|Default Gameplay Effects")
	TSubclassOf<UGameplayEffect> GE_DynamicTag;

	UPROPERTY(EditDefaultsOnly, Category = "Battle|Default Gameplay Effects")
	TSubclassOf<UGameplayEffect> GE_InputCharging;

	UPROPERTY(EditDefaultsOnly, Category = "Battle|Default Gameplay Effects")
	TSubclassOf<UGameplayEffect> GE_Damage;
};