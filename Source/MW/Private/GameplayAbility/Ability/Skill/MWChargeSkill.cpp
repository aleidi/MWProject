#include "GameplayAbility/Ability/Skill/MWChargeSkill.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystemComponent.h"
#include "Animation/AnimInstance.h"
#include "GameplayAbility/Task/AbilityTask_ChargeTick.h"
#include "Gameplay/MWGameplayTags.h"
#include "MWLogChannels.h"
#include "System/MWConsoleVars.h"
#include "Util/UEDebugUtils.h"

UMWChargeSkill::UMWChargeSkill(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	ActivationPolicy = EMWAbilityActivationPolicy::OnInputTriggered;
}

void UMWChargeSkill::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (TriggerEventData && bHasBlueprintActivateFromEvent)
	{
		K2_ActivateAbilityFromEvent(*TriggerEventData);
	}
	else if (bHasBlueprintActivate)
	{
		K2_ActivateAbility();
	}
	else if (bHasBlueprintActivateFromEvent)
	{
		UE_LOG(LogMWAbilitySystem, Warning, TEXT("Ability %s expects event data but none is being supplied. Use 'Activate Ability' instead of 'Activate Ability From Event' in the Blueprint."), *GetName());
		constexpr bool bReplicateEndAbility = false;
		constexpr bool bWasCancelled = true;
		EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	}

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	StartChargeTask();
}

void UMWChargeSkill::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ClearMontageTask();
	ClearChargeTask();

	CurrentChargeValue = 0.f;
	bWasFullyCharged = false;

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UMWChargeSkill::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);

	if (IsChargeTaskActive())
	{
		ChargeTask->SetCharging(true);
	}
	else
	{
		StartChargeTask();
	}
}

void UMWChargeSkill::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);

	if (bWasFullyCharged)
	{
		if (CanPlayAbilityAnimation())
		{
			PlayAbilityAnimation();

			ClearChargeTask();

			return;
		}
	}

	if (IsChargeTaskActive())
	{
		ChargeTask->SetCharging(false);
	}
}

void UMWChargeSkill::OnChargeValueChanged(float NewValue, float Percent)
{
	CurrentChargeValue = NewValue;
	UE_SCREEN_PRINT_CVAR(MWConsoleVars::CVarShowSkillDebug, 0.f, FColor::Yellow, TEXT("Charge Value Changed: %.1f (%.0f%%)"), NewValue, Percent * 100.f);
}

void UMWChargeSkill::OnChargeFull()
{
	if (!bWasFullyCharged)
	{
		bWasFullyCharged = true;
	}
}

void UMWChargeSkill::OnChargeZero()
{
	bWasFullyCharged = false;

	if (!bEndAbilityOnDepleted && IsChargeTaskActive())
	{
		ClearChargeTask();
	}
	else if (bEndAbilityOnDepleted)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}

void UMWChargeSkill::StartChargeTask()
{
	ChargeTask = UAbilityTask_ChargeTick::StartChargeTask(this, MaxChargeValue, ChargeRate, DischargeRate);
	if (ChargeTask)
	{
		ChargeTask->OnValueChanged.AddDynamic(this, &UMWChargeSkill::OnChargeValueChanged);
		ChargeTask->OnFull.AddDynamic(this, &UMWChargeSkill::OnChargeFull);
		ChargeTask->OnZero.AddDynamic(this, &UMWChargeSkill::OnChargeZero);
		ChargeTask->ReadyForActivation();

		ChargeTask->SetCharging(true);
	}
}

void UMWChargeSkill::ClearChargeTask()
{
	if (ChargeTask)
	{
		ChargeTask->EndTask();
		ChargeTask = nullptr;
	}
}

bool UMWChargeSkill::IsChargeTaskActive() const
{
	return IsActive() && ChargeTask != nullptr;
}

bool UMWChargeSkill::CanPlayAbilityAnimation() const
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
		return false;
	}

	// 正在播放动画，检查是否处于连携窗口
	return asc->HasMatchingGameplayTag(MWGameplayTags::Ability_Skill_Chainable);
}

void UMWChargeSkill::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UMWChargeSkill::OnMontageCancelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UMWChargeSkill::OnMontageInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
