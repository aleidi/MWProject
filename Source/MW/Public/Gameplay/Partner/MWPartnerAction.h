#pragma once

#include "GameplayAbility/MWGameplayAbility.h"
#include "MWPartnerAction.generated.h"

class UAnimMontage;

UCLASS(Blueprintable)
class MW_API UMWPartnerAction : public UMWGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	UFUNCTION()
	virtual void OnAnimCanceled();
	UFUNCTION()
	virtual void OnAnimInterrupted();
	UFUNCTION()
	virtual void OnAnimCompleted();

protected:
	TObjectPtr<UAnimMontage> ActionAnim;
};