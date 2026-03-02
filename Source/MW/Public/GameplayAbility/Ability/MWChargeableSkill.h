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
 * @brief 蓄力完成 → 捕获目标（攻击判定）→ 播放动画 → AnimNotify触发 → 计算并应用伤害
 *
 * @note
 */
UCLASS()
class UMWChargeableSkill : public UMWChargeableAbility
{
	GENERATED_BODY()
	
public:
	//UMWChargeableSkill();

	virtual void OnInputReleased(float ChargeTime) override;

	/** Called by AnimNotify to trigger damage calculation at specific timestamps */
	virtual void OnDamageNotify(FGameplayTag DamageEventTag, float DamageMultiplier, int32 HitIndex);

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

	/** Called to capture targets when charge completes (攻击判定) */
	virtual void CaptureTargets();

	/** Apply damage to captured targets */
	virtual void ApplyDamageToTargets(FGameplayTag DamageEventTag, float DamageMultiplier, int32 HitIndex);

	/** Create a damage effect spec based on charge stage and multipliers */
	virtual FGameplayEffectSpecHandle MakeDamageEffectSpec(float DamageMultiplier);

	/** Clear captured target data */
	virtual void ClearCapturedTargets();

protected:
	// Chargeable Skill Data
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Chargeable Skill")
	TObjectPtr<UMWChargeableSkillData> Data;

	FActiveGameplayEffectHandle  ChargingEffectHandle;

	// Tracks the charge stage tag added by this ability for precise cleanup to support Combo system. 
	// この Ability が追加したチャージ段階タグを追跡し、Combo システムをサポートするために正確にクリーンアップする
	FGameplayTag AddedChargeStageTag = FGameplayTag::EmptyTag;

	UPROPERTY()
	FGameplayAbilityTargetDataHandle CapturedTargetData;

	UPROPERTY()
	EMWInputChargeStage CurrentChargeStage = EMWInputChargeStage::NoCharge;
};