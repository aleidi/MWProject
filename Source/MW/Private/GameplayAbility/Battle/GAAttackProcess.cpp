#include "GameplayAbility/Battle/GAAttackProcess.h"
#include "AbilitySystemComponent.h"

void UGAAttackProcess::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (auto* asc = GetAbilitySystemComponentFromActorInfo())
	{
		//asc->PlayMontage(this, ActivationInfo, MontageToPlay, 1.f);
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, false, true);

}

void UGAAttackProcess::OnEndAbility(const FGameplayAbilitySpecHandle handle, const FGameplayAbilityActorInfo* actorInfo, const FGameplayAbilityActivationInfo activationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	//throw std::logic_error("The method or operation is not implemented.");
}
