#pragma once

#include "Data/Skill/MWSkillTable.h"
#include "GameplayAbility/Ability/Skill/MWSkillBase.h"
#include "MWRangeAdaptiveSkill.generated.h"

class AActor;

UENUM(BlueprintType)
enum class EMWSkillRangeForm : uint8
{
	/** Close-range form (e.g. melee combo). */
	Close,

	/** Long-range form (e.g. projectile shot). */
	Far,
};

/**
 * UMWRangeAdaptiveSkill
 *
 * Selects close/far skill form based on distance to the resolved target at cast time.
 */
UCLASS()
class MW_API UMWRangeAdaptiveSkill : public UMWSkillBase
{
	GENERATED_BODY()

protected:
	//~UMWGameplayAbility interface
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~End of UMWGameplayAbility interface

private:
	/** Resolves range-adaptive config by SkillId. Returns nullptr when this skill is not range-adaptive. */
	const FMWSkillRangeAdaptiveConfig* ResolveRangeAdaptiveConfig(const FMWSkillCastCommand& InCommand) const;

	/** Resolves the best target actor for distance decision (current implementation: nearest pawn in radius). */
	AActor* ResolveBestTargetActor(const FMWSkillRangeAdaptiveConfig& InConfig) const;

	/** Decides close/far form with hysteresis to avoid boundary jitter. */
	EMWSkillRangeForm DecideRangeForm(float InDistance, const FMWSkillRangeAdaptiveConfig& InConfig);

	/** Resolves form when no target is available. Can request cast failure via out flag. */
	EMWSkillRangeForm ResolveNoTargetForm(const FMWSkillRangeAdaptiveConfig& InConfig, bool& bOutShouldFailCast) const;

	/** Writes resolved form back to cast command (mainly montage section override). */
	void ApplyRangeFormToCommand(EMWSkillRangeForm InForm, const FMWSkillRangeAdaptiveConfig& InConfig, FMWSkillCastCommand& InOutCommand) const;

private:
	/** Cached target for current ability lifecycle. */
	UPROPERTY(Transient)
	TObjectPtr<AActor> CachedTargetActor = nullptr;

	/** Last resolved form used by hysteresis logic. */
	EMWSkillRangeForm LastRangeForm = EMWSkillRangeForm::Far;
};
