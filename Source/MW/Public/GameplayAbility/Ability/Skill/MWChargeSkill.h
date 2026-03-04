#pragma once

#include "GameplayAbility/Ability/Skill/MWSkillBase.h"
#include "MWChargeSkill.generated.h"

class UAbilityTask_ChargeTick;
class UAbilityTask_PlayMontageAndWait;
class UAnimMontage;

/**
 * UMWChargeSkill
 * 
 * 实现按住蓄力、松开减少、再次按下继续蓄力的能力基类
 */
UCLASS()
class MW_API UMWChargeSkill : public UMWSkillBase
{
	GENERATED_BODY()

public:
	UMWChargeSkill(const FObjectInitializer& ObjectInitializer);

protected:
	//~UMWGameplayAbility interface
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	//~End of UMWGameplayAbility interface

	UFUNCTION()
	void OnChargeValueChanged(float NewValue, float Percent);

	UFUNCTION()
	void OnChargeFull();

	UFUNCTION()
	void OnChargeZero();

	void StartChargeTask();

	void ClearChargeTask();

	bool IsChargeTaskActive() const;

	/**
	 * 检查当前是否可以播放蓄力完成后的动画。
	 * 当角色没有任何 Montage 正在播放时，直接返回 true；
	 * 若有 Montage 正在播放，则检查 AbilitySystemComponent 是否带有
	 * Ability.Skill.Chainable 标签（连携窗口），有则允许播放，否则返回 false。
	 * TODO:根据后续的实现可能会有所修改
	 */
	virtual bool CanPlayAbilityAnimation() const override;

	virtual void OnMontageCompleted() override;

	virtual void OnMontageCancelled() override;

	virtual void OnMontageInterrupted() override;

public:
	/** 获取当前蓄力值 */
	UFUNCTION(BlueprintPure, Category = "MW|Charge")
	float GetCurrentChargeValue() const { return CurrentChargeValue; }

	/** 获取当前蓄力百分比 */
	UFUNCTION(BlueprintPure, Category = "MW|Charge")
	float GetChargePercent() const { return FMath::Clamp(CurrentChargeValue / MaxChargeValue, 0.f, 1.f); }

	/** 是否蓄力已满 */
	UFUNCTION(BlueprintPure, Category = "MW|Charge")
	bool IsFullyCharged() const { return CurrentChargeValue >= MaxChargeValue; }

protected:
	/** 最大蓄力值 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MW|Charge", meta = (ClampMin = "0.1"))
	float MaxChargeValue = 100.f;

	/** 蓄力速率（每秒增加的值） */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MW|Charge", meta = (ClampMin = "0.1"))
	float ChargeRate = 30.f;

	/** 蓄力衰减速率（每秒减少的值） */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MW|Charge", meta = (ClampMin = "0.1"))
	float DischargeRate = 50.f;

	/** 是否在蓄力值归零后自动结束能力 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MW|Charge")
	bool bEndAbilityOnDepleted = true;

	/** 蓄力Task启动前的延迟时间（秒），用于让同按键的Tap技能优先响应 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MW|Charge", meta = (ClampMin = "0.0"))
	float ChargeStartDelay = 0.15f;

private:
	UPROPERTY()
	TObjectPtr<UAbilityTask_ChargeTick> ChargeTask = nullptr;

	/** 当前蓄力值 */
	float CurrentChargeValue = 0.0f;

	/** 是否已经蓄满 */
	bool bWasFullyCharged = false;
};