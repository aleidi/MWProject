#pragma once

// Include Header
#include "GameplayAbility/Ability/MWChargeableAbility.h"
#include "Data/GameplayAbility/ChargeableSkillData.h"
#include "MWChargeableSkill.generated.h"

// Forward Declare

// Define
class UMWChargeableSkillData;

/*
 * @class UMWChargeableSkill
 * 
 * @brief 
 *
 * @note
 */
UCLASS()
class UMWChargeableSkill : public UMWChargeableAbility
{
	GENERATED_BODY()
	
public:
	//UMWChargeableSkill();

	virtual void OnChargeComplete(float ChargeTime) override;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	EMWInputChargeStage CalculateChargeStage(float ChargeTime) const;

	//virtual void OnMontageCompleted();
	//virtual void OnMontageBlendOut();
	//virtual void OnMontageInterrupted();
	//virtual void OnMontageCancelled();

protected:
	// Chargeable Skill Data
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Chargeable Skill")
	TObjectPtr<UMWChargeableSkillData> Data;

	FActiveGameplayEffectHandle  ChargingEffectHandle;
};