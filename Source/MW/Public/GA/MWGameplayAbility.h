#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "MWGameplayAbility.generated.h"

UCLASS()
class MW_API UMWGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UMWGameplayAbility();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual void OnEndAbility(const FGameplayAbilitySpecHandle handle, const FGameplayAbilityActorInfo* actorInfo, const FGameplayAbilityActivationInfo activationInfo, bool bReplicateEndAbility, bool bWasCancelled) {}
};