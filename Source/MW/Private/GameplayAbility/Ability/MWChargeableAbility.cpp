#include "GameplayAbility/Ability/MWChargeableAbility.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"

void UMWChargeableAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UAbilityTask_WaitInputRelease* waitInputTask = UAbilityTask_WaitInputRelease::WaitInputRelease(this);
	waitInputTask->OnRelease.AddDynamic(this, &UMWChargeableAbility::OnChargeComplete);
	waitInputTask->ReadyForActivation();
}

void UMWChargeableAbility::OnChargeComplete(float ChargeTime)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, FString::Printf(TEXT("Charge Complete! Charge Time: %f"), ChargeTime));

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
