#include "GameplayAbility/Ability/Skill/MWChargeSkillInput.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystemComponent.h"
#include "Animation/AnimInstance.h"
#include "GameplayAbility/Task/AbilityTask_ChargeTick.h"
#include "Gameplay/MWGameplayTags.h"
#include "MWLogChannels.h"

#define DEBUG_PRINT(Time, Format, ...) \
	if (GEngine) \
	{ \
		GEngine->AddOnScreenDebugMessage(-1, Time, FColor::Yellow, FString::Printf(TEXT(Format), ##__VA_ARGS__)); \
	}

UMWChargeSkillInput::UMWChargeSkillInput(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	ActivationPolicy = EMWAbilityActivationPolicy::OnInputTriggered;
}

void UMWChargeSkillInput::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	StartChargeTask();
}

void UMWChargeSkillInput::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ClearChargeTask();

	CurrentChargeValue = 0.f;
	bWasFullyCharged = false;

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UMWChargeSkillInput::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
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

void UMWChargeSkillInput::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);

	if (bWasFullyCharged)
	{
		if (TryActivateSkill())
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, false);

			return;
		}
	}

	if (IsChargeTaskActive())
	{
		ChargeTask->SetCharging(false);
	}
}

void UMWChargeSkillInput::OnChargeValueChanged(float NewValue, float Percent)
{
	CurrentChargeValue = NewValue;
	DEBUG_PRINT(0.f, "Charge Value: %.1f (%.0f%%)", NewValue, Percent * 100.f);
}

void UMWChargeSkillInput::OnChargeFull()
{
	if (!bWasFullyCharged)
	{
		bWasFullyCharged = true;
	}
}

void UMWChargeSkillInput::OnChargeZero()
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

void UMWChargeSkillInput::StartChargeTask()
{
	ChargeTask = UAbilityTask_ChargeTick::StartChargeTask(this, MaxChargeValue, ChargeRate, DischargeRate);
	if (ChargeTask)
	{
		ChargeTask->OnValueChanged.AddDynamic(this, &UMWChargeSkillInput::OnChargeValueChanged);
		ChargeTask->OnFull.AddDynamic(this, &UMWChargeSkillInput::OnChargeFull);
		ChargeTask->OnZero.AddDynamic(this, &UMWChargeSkillInput::OnChargeZero);
		ChargeTask->ReadyForActivation();

		ChargeTask->SetCharging(true);
	}
}

void UMWChargeSkillInput::ClearChargeTask()
{
	if (ChargeTask)
	{
		ChargeTask->EndTask();
		ChargeTask = nullptr;
	}
}

bool UMWChargeSkillInput::IsChargeTaskActive() const
{
	return IsActive() && ChargeTask != nullptr;
}

bool UMWChargeSkillInput::TryActivateSkill()
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (!ASC || !ChargeReleasedEventTag.IsValid())
	{
		return false;
	}

	FGameplayEventData EventData;
	EventData.OptionalObject = this;

	return ASC->HandleGameplayEvent(ChargeReleasedEventTag, &EventData) > 0;
}
