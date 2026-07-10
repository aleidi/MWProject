#include "GameplayAbility/Ability/Skill/MWSkillBase.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystemComponent.h"
#include "Animation/AnimInstance.h"
#include "Character/MWSkillComponent.h"
#include "GameplayAbility/Ability/Skill/MWSkillCastPayload.h"
#include "Gameplay/MWGameplayTags.h"
UE_DISABLE_OPTIMIZATION
void UMWSkillBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	FMWSkillCastCommand castCommand;
	if (!TryResolveCastCommand(TriggerEventData, castCommand))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);

		return;
	}

	if (!TryCommitAndPlayFromCommand(Handle, ActorInfo, ActivationInfo, castCommand))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);

		return;
	}
}

void UMWSkillBase::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	PendingCommitSkillId = INDEX_NONE;

	ClearMontageTask();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UMWSkillBase::CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags /*= nullptr*/) const
{
	if (!Super::CheckCost(Handle, ActorInfo, OptionalRelevantTags))
	{
		return false;
	}

	if (PendingCommitSkillId == INDEX_NONE)
	{
		return true;
	}

	AActor* sourceActor = GetAvatarActorFromActorInfo();
	if (!sourceActor)
	{
		sourceActor = GetOwningActorFromActorInfo();
	}

	UMWSkillComponent* skillComponent = sourceActor ? sourceActor->FindComponentByClass<UMWSkillComponent>() : nullptr;

	return skillComponent && skillComponent->CanConsumeSkillUse(PendingCommitSkillId, 1);
}

void UMWSkillBase::ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	Super::ApplyCost(Handle, ActorInfo, ActivationInfo);

	if (PendingCommitSkillId == INDEX_NONE)
	{
		return;
	}

	AActor* sourceActor = GetAvatarActorFromActorInfo();
	if (!sourceActor)
	{
		sourceActor = GetOwningActorFromActorInfo();
	}

	UMWSkillComponent* skillComponent = sourceActor ? sourceActor->FindComponentByClass<UMWSkillComponent>() : nullptr;
	if (skillComponent)
	{
		skillComponent->ConsumeSkillUse(PendingCommitSkillId, 1);
	}
}

void UMWSkillBase::ClearMontageTask()
{
	if (MontageTask)
	{
		MontageTask->EndTask();
		MontageTask = nullptr;
	}
}

bool UMWSkillBase::CanPlayAbilityAnimation() const
{
	const FGameplayAbilityActorInfo* actorInfo = GetCurrentActorInfo();
	const UAnimInstance* animInst = actorInfo ? actorInfo->GetAnimInstance() : nullptr;
	if (!animInst)
	{
		return false;
	}

	if (!animInst->IsAnyMontagePlaying())
	{
		return true;
	}

	const UAbilitySystemComponent* asc = GetAbilitySystemComponentFromActorInfo();
	return !asc || asc->HasMatchingGameplayTag(MWGameplayTags::Ability_Skill_Chainable);
}

bool UMWSkillBase::TryResolveCastCommand(const FGameplayEventData* TriggerEventData, FMWSkillCastCommand& OutCommand) const
{
	if (TriggerEventData && TriggerEventData->OptionalObject)
	{
		const UMWSkillCastPayload* payload = Cast<UMWSkillCastPayload>(TriggerEventData->OptionalObject);
		if (payload)
		{
			OutCommand = payload->Command;
			return true;
		}
	}

	FGameplayTag inputTag;
	if (TriggerEventData && TriggerEventData->InstigatorTags.Num() > 0)
	{
		inputTag = TriggerEventData->InstigatorTags.First();
	}
	else if (const FGameplayAbilitySpec* spec = GetCurrentAbilitySpec())
	{
		const FGameplayTagContainer& sourceTags = spec->GetDynamicSpecSourceTags();
		if (sourceTags.Num() > 0)
		{
			inputTag = sourceTags.First();
		}
	}

	if (!inputTag.IsValid())
	{
		return false;
	}

	const AActor* sourceActor = GetAvatarActorFromActorInfo();
	if (!sourceActor)
	{
		sourceActor = GetOwningActorFromActorInfo();
	}

	const UMWSkillComponent* skillComponent = sourceActor ? sourceActor->FindComponentByClass<UMWSkillComponent>() : nullptr;
	return skillComponent ? skillComponent->TryBuildCastCommandFromInputTag(inputTag, OutCommand) : false;
}

bool UMWSkillBase::TryResolveSkillPresentation(const FMWSkillCastCommand& InCommand)
{
	AbilityAnim = nullptr;
	MontageSectionName = NAME_None;

	const AActor* sourceActor = GetAvatarActorFromActorInfo();
	if (!sourceActor)
	{
		sourceActor = GetOwningActorFromActorInfo();
	}

	const UMWSkillComponent* skillComponent = sourceActor ? sourceActor->FindComponentByClass<UMWSkillComponent>() : nullptr;
	if (!skillComponent)
	{
		return false;
	}

	UAnimMontage* resolvedMontage = nullptr;
	if (!skillComponent->TryResolveMontageData(InCommand, resolvedMontage, MontageSectionName))
	{
		return false;
	}

	AbilityAnim = resolvedMontage;
	return AbilityAnim != nullptr;
}

void UMWSkillBase::SetPendingCommitSkillId(int32 InSkillId)
{
	PendingCommitSkillId = InSkillId;
}

bool UMWSkillBase::TryCommitAndPlayFromCommand(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FMWSkillCastCommand& InCommand)
{
	if (!TryResolveSkillPresentation(InCommand))
	{
		return false;
	}

	if (!CanPlayAbilityAnimation())
	{
		return false;
	}

	SetPendingCommitSkillId(InCommand.SkillId);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		return false;
	}

	PlayAbilityAnimation();

	return true;
}

void UMWSkillBase::PlayAbilityAnimation()
{
	if (!AbilityAnim)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	ClearMontageTask();

	MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		AbilityAnim,
		MontagePlayRate,
		MontageSectionName);

	if (!MontageTask)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	MontageTask->OnCompleted.AddDynamic(this, &UMWSkillBase::OnMontageCompleted);
	MontageTask->OnCancelled.AddDynamic(this, &UMWSkillBase::OnMontageCancelled);
	MontageTask->OnInterrupted.AddDynamic(this, &UMWSkillBase::OnMontageInterrupted);
	MontageTask->ReadyForActivation();
}

void UMWSkillBase::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UMWSkillBase::OnMontageCancelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UMWSkillBase::OnMontageInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
UE_ENABLE_OPTIMIZATION
