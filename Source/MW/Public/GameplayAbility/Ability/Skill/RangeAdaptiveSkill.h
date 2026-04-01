#pragma once

#include "GameplayAbility/Ability/Skill/MWSkillBase.h"
#include "RangeAdaptiveSkill.generated.h"

/**
 * URangeAdaptiveSkill
 * 
 * 按键释放能力技能
 */
UCLASS()
class MW_API URangeAdaptiveSkill : public UMWSkillBase
{
	GENERATED_BODY()

protected:
	//~UMWGameplayAbility interface
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~End of UMWGameplayAbility interface
};