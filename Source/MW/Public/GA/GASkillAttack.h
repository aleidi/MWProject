#pragma once

#include "CoreMinimal.h"
#include "GA/MWGameplayAbility.h"
#include "GASKillAttack.generated.h"

UCLASS()
class MW_API UGASKillAttack : public UMWGameplayAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void OnEndAbility(const FGameplayAbilitySpecHandle handle, const FGameplayAbilityActorInfo* actorInfo, const FGameplayAbilityActivationInfo activationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* MontageToPlay;
};