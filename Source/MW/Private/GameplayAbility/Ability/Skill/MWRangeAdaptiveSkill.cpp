#include "GameplayAbility/Ability/Skill/MWRangeAdaptiveSkill.h"

#include "Character/MWSkillComponent.h"
#include "Data/Skill/MWSkillDataManager.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "GameFramework/Pawn.h"

void UMWRangeAdaptiveSkill::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	FMWSkillCastCommand castCommand;
	if (!TryResolveCastCommand(TriggerEventData, castCommand))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);

		return;
	}

	// Charge casts must keep base skill flow and must not be converted to close/far.
	if (castCommand.Variant == EMWSkillCastVariant::Charge)
	{
		Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

		return;
	}

	const FMWSkillRangeAdaptiveConfig* adaptiveConfig = ResolveRangeAdaptiveConfig(castCommand);
	if (!adaptiveConfig)
	{
		Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

		return;
	}

	bool bShouldFailCast = false;
	EMWSkillRangeForm resolvedForm = EMWSkillRangeForm::Far;

	CachedTargetActor = ResolveBestTargetActor(*adaptiveConfig);
	if (CachedTargetActor)
	{
		const AActor* sourceActorForDistance = GetAvatarActorFromActorInfo() ? GetAvatarActorFromActorInfo() : GetOwningActorFromActorInfo();
		if (!sourceActorForDistance)
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, true);

			return;
		}

		const float distance = FVector::Dist2D(sourceActorForDistance->GetActorLocation(), CachedTargetActor->GetActorLocation());
		resolvedForm = DecideRangeForm(distance, *adaptiveConfig);
	}
	else
	{
		resolvedForm = ResolveNoTargetForm(*adaptiveConfig, bShouldFailCast);
		if (bShouldFailCast)
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, true);

			return;
		}
	}

	ApplyRangeFormToCommand(resolvedForm, *adaptiveConfig, castCommand);

	if (!TryCommitAndPlayFromCommand(Handle, ActorInfo, ActivationInfo, castCommand))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);

		return;
	}
}

void UMWRangeAdaptiveSkill::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	CachedTargetActor = nullptr;
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

const FMWSkillRangeAdaptiveConfig* UMWRangeAdaptiveSkill::ResolveRangeAdaptiveConfig(const FMWSkillCastCommand& InCommand) const
{
	if (InCommand.SkillId == INDEX_NONE)
	{
		return nullptr;
	}

	const UMWSkillDataManager* skillDataMgr = GET_SKILLDATAMGR(this);
	const FMWSkillTable* skillRow = skillDataMgr ? skillDataMgr->FindSkillRow(InCommand.SkillId) : nullptr;
	if (!skillRow || !skillRow->bIsRangeAdaptive)
	{
		return nullptr;
	}

	return &skillRow->RangeAdaptiveConfig;
}

AActor* UMWRangeAdaptiveSkill::ResolveBestTargetActor(const FMWSkillRangeAdaptiveConfig& InConfig) const
{
	const AActor* sourceActor = GetAvatarActorFromActorInfo() ? GetAvatarActorFromActorInfo() : GetOwningActorFromActorInfo();
	UWorld* world = GetWorld();
	if (!sourceActor || !world)
	{
		return nullptr;
	}

	const float searchRadiusSq = InConfig.TargetSearchRadius * InConfig.TargetSearchRadius;
	AActor* bestTarget = nullptr;
	float bestDistanceSq = searchRadiusSq;

	for (TActorIterator<APawn> it(world); it; ++it)
	{
		APawn* candidate = *it;
		if (!IsValid(candidate) || candidate == sourceActor)
		{
			continue;
		}

		const float distanceSq = FVector::DistSquared2D(sourceActor->GetActorLocation(), candidate->GetActorLocation());
		if (distanceSq <= bestDistanceSq)
		{
			bestDistanceSq = distanceSq;
			bestTarget = candidate;
		}
	}

	return bestTarget;
}

EMWSkillRangeForm UMWRangeAdaptiveSkill::DecideRangeForm(float InDistance, const FMWSkillRangeAdaptiveConfig& InConfig) const
{
	const float nearDistance = FMath::Max(0.0f, InConfig.NearEnterDistance);

	if (InDistance <= nearDistance)
	{
		return EMWSkillRangeForm::Close;
	}

	return EMWSkillRangeForm::Far;
}

EMWSkillRangeForm UMWRangeAdaptiveSkill::ResolveNoTargetForm(const FMWSkillRangeAdaptiveConfig& InConfig, bool& bOutShouldFailCast) const
{
	bOutShouldFailCast = false;

	switch (InConfig.NoTargetPolicy)
	{
	case EMWSkillNoTargetPolicy::FailCast:
		bOutShouldFailCast = true;
		return EMWSkillRangeForm::Far;

	case EMWSkillNoTargetPolicy::UseClose:
		return EMWSkillRangeForm::Close;

	case EMWSkillNoTargetPolicy::UseFar:
		return EMWSkillRangeForm::Far;

	default:
		bOutShouldFailCast = true;
		return EMWSkillRangeForm::Far;
	}
}

void UMWRangeAdaptiveSkill::ApplyRangeFormToCommand(EMWSkillRangeForm InForm, const FMWSkillRangeAdaptiveConfig& InConfig, FMWSkillCastCommand& InOutCommand) const
{
	InOutCommand.Variant = EMWSkillCastVariant::Normal;

	if (InForm == EMWSkillRangeForm::Close)
	{
		InOutCommand.OverrideSection = InConfig.CloseSectionName != NAME_None ? InConfig.CloseSectionName : TEXT("Close");
	}
	else
	{
		InOutCommand.OverrideSection = InConfig.FarSectionName != NAME_None ? InConfig.FarSectionName : TEXT("Far");
	}
}
