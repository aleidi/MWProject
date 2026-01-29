#pragma once

// Include Header
#include "GameplayAbility/MWGameplayAbility.h"
#include "MWAbilityInputRouter.generated.h"

// Forward Declare

// Define

/*
 * @class UMWAbilityInputRouter
 * 
 * @brief Receive the input signal and route it to the appropriate ability.
 *
 * @note
 */
UCLASS()
class UMWAbilityInputRouter : public UMWGameplayAbility
{
	GENERATED_BODY()
	
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
};