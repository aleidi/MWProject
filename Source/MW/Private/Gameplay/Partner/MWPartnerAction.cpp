#include "Gameplay/Partner/MWPartnerAction.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Gameplay/Partner/MWPartnerComponent.h"
#include "Gameplay/Partner/MWPartnerStatics.h"

void UMWPartnerAction::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UAbilityTask_PlayMontageAndWait* playMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, ActionAnim);
	playMontageTask->OnCompleted.AddDynamic(this, &UMWPartnerAction::OnAnimCompleted);
	playMontageTask->OnInterrupted.AddDynamic(this, &UMWPartnerAction::OnAnimInterrupted);
	playMontageTask->OnCancelled.AddDynamic(this, &UMWPartnerAction::OnAnimCanceled);
}

void UMWPartnerAction::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UMWPartnerAction::OnAnimCanceled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UMWPartnerAction::OnAnimInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UMWPartnerAction::OnAnimCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
