#pragma once

// Include Header
#include "GameplayAbility/Ability/MWChargeableAbility.h"
#include "Data/GameplayAbility/ChargeableSkillData.h"
#include "MWChargeableSkill.generated.h"

// Forward Declare

// Define
struct FGameplayTag;
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
	
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled);

	EMWInputChargeStage CalculateChargeStage(float ChargeTime) const;

	UFUNCTION()
	virtual void OnMontageCompleted();
	UFUNCTION()
	virtual void OnMontageBlendIn();
	UFUNCTION()
	virtual void OnMontageBlendOut();
	UFUNCTION()
	virtual void OnMontageInterrupted();
	UFUNCTION()
	virtual void OnMontageCancelled();

	void ClearTagForThisAblity();

protected:
	// Chargeable Skill Data
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Chargeable Skill")
	TObjectPtr<UMWChargeableSkillData> Data;

	FActiveGameplayEffectHandle  ChargingEffectHandle;

	// Tracks the charge stage tag added by this ability for precise cleanup to support Combo system. 
	// この Ability が追加したチャージ段階タグを追跡し、Combo システムをサポートするために正確にクリーンアップする
	FGameplayTag AddedChargeStageTag = FGameplayTag::EmptyTag;
};