#pragma once

// Include Header
#include "GameplayAbility/MWGameplayAbility.h"
#include "MWChargeableAbility.generated.h"

// Forward Declare

// Define

/*
 * @class UMWChargeableAbility
 * 
 * @brief 
 *
 * @note
 */
UCLASS()
class UMWChargeableAbility : public UMWGameplayAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void OnChargeComplete(float ChargeTime);
};