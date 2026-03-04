#include "GameplayAbility/Ability/Skill/MWSkillBase.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystemComponent.h"
#include "Animation/AnimInstance.h"
#include "GameplayAbility/Task/AbilityTask_ChargeTick.h"
#include "Gameplay/MWGameplayTags.h"

UMWSkillBase::UMWSkillBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UMWSkillBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// TODO:Get Animation Data

	if(CanPlayAbilityAnimation())
	{
		PlayAbilityAnimation();
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}
}

void UMWSkillBase::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
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
	const FGameplayAbilityActorInfo* currentActorInfo = GetCurrentActorInfo();

	const UAnimInstance* animInst = currentActorInfo ? currentActorInfo->GetAnimInstance() : nullptr;
	if (!animInst)
	{
		return false;
	}

	const bool bIsPlayingMontage = animInst->IsAnyMontagePlaying();
	if (!bIsPlayingMontage)
	{
		return true;
	}

	const UAbilitySystemComponent* asc = GetAbilitySystemComponentFromActorInfo();
	if (!asc)
	{
		return true;
	}

	// 正在播放动画，检查是否处于连携窗口
	return asc->HasMatchingGameplayTag(MWGameplayTags::Ability_Skill_Chainable);
}

void UMWSkillBase::PlayAbilityAnimation()
{
	if (!AbilityAnim)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);

		return;
	}

	MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		AbilityAnim,
		MontagePlayRate,
		MontageSectionName
	);

	if (MontageTask)
	{
		MontageTask->OnCompleted.AddDynamic(this, &UMWSkillBase::OnMontageCompleted);
		MontageTask->OnCancelled.AddDynamic(this, &UMWSkillBase::OnMontageCancelled);
		MontageTask->OnInterrupted.AddDynamic(this, &UMWSkillBase::OnMontageInterrupted);
		MontageTask->ReadyForActivation();
	}
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
