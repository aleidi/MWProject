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
	// Gameplay effect used to apply blocking skill cast.
	UPROPERTY(EditDefaultsOnly, Category = "Battle|Default Gameplay Effects", meta = (DisplayName = "Gameplay Effect Block Cast Skill"))
	TSubclassOf<UGameplayEffect> GEBlockCastSkill;

	// Gameplay effect used to apply unblocking skill cast.
	UPROPERTY(EditDefaultsOnly, Category = "Battle|Default Gameplay Effects", meta = (DisplayName = "Gameplay Effect Unblock Cast Skill"))
	TSubclassOf<UGameplayEffect> GEUnblockCastSkill;

	// Gameplay effect used to enable skill combo. 
	UPROPERTY(EditDefaultsOnly, Category = "Battle|Default Gameplay Effects", meta = (DisplayName = "Gameplay Effect Skill Combo"))
	TSubclassOf<UGameplayEffect> GESkillCombo;

	// Gameplay effect used to be appled when getting perfect combo in skill.
	UPROPERTY(EditDefaultsOnly, Category = "Battle|Default Gameplay Effects", meta = (DisplayName = "Gameplay Effect Skill Perfect Combo"))
	TSubclassOf<UGameplayEffect> GESkillPerfectCombo;

	// Gameplay effect used to add and remove dynamic tags.
	UPROPERTY(EditDefaultsOnly, Category = "Battle|Default Gameplay Effects")
	TSubclassOf<UGameplayEffect> DynamicTagGameplayEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Battle|Default Gameplay Effects")
	TSubclassOf<UGameplayEffect> GEInputCharging;

	UPROPERTY(EditDefaultsOnly, Category = "Battle|Scene Setting")
	TObjectPtr<UMWBattleSceneSetting> BattleSceneSetting;
};
