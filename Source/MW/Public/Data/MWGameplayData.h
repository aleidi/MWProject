#pragma once

#include "Engine/DataAsset.h"
#include "MWGameplayData.generated.h"

class UGameplayEffect;
class UMWBattleSceneSetting;

/**
 * UMWGameplayData
 *
 */
UCLASS(BlueprintType, Const, Meta = (DisplayName = "MW Gameplay Data", ShortTooltip = "Data asset containing gaameplay data."))
class UMWGameplayData : public UDataAsset
{
	GENERATED_BODY()

public:

	UMWGameplayData();

	// Quick access to data.
	static const UMWGameplayData& Get();

public:

	// Gameplay effect used to apply blocking skill cast.
	UPROPERTY(EditDefaultsOnly, Category = "Battle|Default Gameplay Effects", meta = (DisplayName = "Gameplay Effect Block Cast Skill"))
	TSoftClassPtr<UGameplayEffect> GEBlockCastSkill;

	// Gameplay effect used to apply unblocking skill cast.
	UPROPERTY(EditDefaultsOnly, Category = "Battle|Default Gameplay Effects", meta = (DisplayName = "Gameplay Effect Unblock Cast Skill"))
	TSoftClassPtr<UGameplayEffect> GEUnblockCastSkill;

	// Gameplay effect used to enable skill combo. 
	UPROPERTY(EditDefaultsOnly, Category = "Battle|Default Gameplay Effects", meta = (DisplayName = "Gameplay Effect Skill Combo"))
	TSoftClassPtr<UGameplayEffect> GESkillCombo;

	// Gameplay effect used to be appled when getting perfect combo in skill.
	UPROPERTY(EditDefaultsOnly, Category = "Battle|Default Gameplay Effects", meta = (DisplayName = "Gameplay Effect Skill Perfect Combo"))
	TSoftClassPtr<UGameplayEffect> GESkillPerfectCombo;

	// Gameplay effect used to add and remove dynamic tags.
	UPROPERTY(EditDefaultsOnly, Category = "Battle|Default Gameplay Effects")
	TSoftClassPtr<UGameplayEffect> DynamicTagGameplayEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Battle|Scene Setting")
	TObjectPtr<UMWBattleSceneSetting> BattleSceneSetting;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Battle|Party", meta = (ClampMin = 0))
	int32 TeamsNumber = 3;
};
