#pragma once

#include "Engine/DataAsset.h"
#include "MWGameplayData.generated.h"

class UGameplayEffect;
class UMWBattleSceneSetting;

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
	// Gameplay effect used to add and remove dynamic tags.
	UPROPERTY(EditDefaultsOnly, Category = "Battle|Default Gameplay Effects")
	TSubclassOf<UGameplayEffect> GE_DynamicTag;

	UPROPERTY(EditDefaultsOnly, Category = "Battle|Default Gameplay Effects")
	TSubclassOf<UGameplayEffect> GE_InputCharging;

	UPROPERTY(EditDefaultsOnly, Category = "Battle|Default Gameplay Effects")
	TSubclassOf<UGameplayEffect> GE_Damage;

	UPROPERTY(EditDefaultsOnly, Category = "Battle|Scene Setting")
	TObjectPtr<UMWBattleSceneSetting> BattleSceneSetting;
};
