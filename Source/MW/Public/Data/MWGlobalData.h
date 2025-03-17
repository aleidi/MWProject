#pragma once

#include "Engine/DataAsset.h"
#include "MWGlobalData.generated.h"

class UGameplayEffect;
class UObject;

/**
 * UMWGlobalData
 *
 *	Non-mutable data asset that contains global game data.
 */
UCLASS(BlueprintType, Const, Meta = (DisplayName = "MW Global Data", ShortTooltip = "Data asset containing global game data."))
class UMWGlobalData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UMWGlobalData();

	// Returns the loaded game data.
	static const UMWGlobalData& Get();

public:

	// Gameplay effect used to apply blocking skill cast.
	UPROPERTY(EditDefaultsOnly, Category = "Default Gameplay Effects", meta = (DisplayName = "Gameplay Effect Block Cast Skill"))
	TSoftClassPtr<UGameplayEffect> GEBlockCastSkill;

	// Gameplay effect used to apply unblocking skill cast.
	UPROPERTY(EditDefaultsOnly, Category = "Default Gameplay Effects", meta = (DisplayName = "Gameplay Effect Unblock Cast Skill"))
	TSoftClassPtr<UGameplayEffect> GEUnblockCastSkill;

	// Gameplay effect used to enable skill combo. 
	UPROPERTY(EditDefaultsOnly, Category = "Default Gameplay Effects", meta = (DisplayName = "Gameplay Effect Skill Combo"))
	TSoftClassPtr<UGameplayEffect> GESkillCombo;

	// Gameplay effect used to be appled when getting perfect combo in skill.
	UPROPERTY(EditDefaultsOnly, Category = "Default Gameplay Effects", meta = (DisplayName = "Gameplay Effect Skill Perfect Combo"))
	TSoftClassPtr<UGameplayEffect> GESkillPerfectCombo;

	// Gameplay effect used to add and remove dynamic tags.
	UPROPERTY(EditDefaultsOnly, Category = "Default Gameplay Effects")
	TSoftClassPtr<UGameplayEffect> DynamicTagGameplayEffect;
};
