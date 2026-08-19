#pragma once

#include "GameplayAbility/MWGameplayAbility.h"
#include "MWChargeSkillInput.generated.h"

class UAbilityTask_ChargeTick;

/**
 * UMWChargeSkillInput
 * 
 * 長押しでチャージし、離すと減少、再度押すとチャージを再開するアビリティ。
 */
UCLASS()
class MW_API UMWChargeSkillInput : public UMWGameplayAbility
{
	GENERATED_BODY()

public:
	UMWChargeSkillInput(const FObjectInitializer& ObjectInitializer);

protected:
	//~UMWGameplayAbilityインターフェース
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	//~UMWGameplayAbilityインターフェース終了

	UFUNCTION()
	void OnChargeValueChanged(float NewValue, float Percent);

	UFUNCTION()
	void OnChargeFull();

	UFUNCTION()
	void OnChargeZero();

	void StartChargeTask();

	void ClearChargeTask();

	bool IsChargeTaskActive() const;

	bool TryActivateSkill();

public:
	/** 現在のチャージ値を取得します。 */
	UFUNCTION(BlueprintPure, Category = "MW|Charge")
	float GetCurrentChargeValue() const { return CurrentChargeValue; }

	/** 現在のチャージ率を取得します。 */
	UFUNCTION(BlueprintPure, Category = "MW|Charge")
	float GetChargePercent() const { return FMath::Clamp(CurrentChargeValue / MaxChargeValue, 0.f, 1.f); }

	/** チャージが最大値に達しているかを返します。 */
	UFUNCTION(BlueprintPure, Category = "MW|Charge")
	bool IsFullyCharged() const { return CurrentChargeValue >= MaxChargeValue; }

protected:
	/** 最大チャージ値。 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MW|Charge", meta = (ClampMin = "0.1"))
	float MaxChargeValue = 100.f;

	/** チャージ速度（1秒あたりの増加量）。 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MW|Charge", meta = (ClampMin = "0.1"))
	float ChargeRate = 70.f;

	/** チャージ減衰速度（1秒あたりの減少量）。 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MW|Charge", meta = (ClampMin = "0.1"))
	float DischargeRate = 90.f;

	/** チャージ値がゼロになった時点でアビリティを自動終了するか。 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MW|Charge")
	bool bEndAbilityOnDepleted = true;

	/** チャージTask開始前の遅延時間（秒）。同じボタンのTapスキルを優先して反応させるために使用します。 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MW|Charge", meta = (ClampMin = "0.0"))
	float ChargeStartDelay = 0.15f;

	/** チャージ完了時に送信するイベントTag。対象AbilityのAbilityTriggersに同じTagを設定する必要があります。 */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MW|Charge")
    FGameplayTag ChargeReleasedEventTag;

private:
	UPROPERTY()
	TObjectPtr<UAbilityTask_ChargeTick> ChargeTask = nullptr;

	/** 現在のチャージ値。 */
	float CurrentChargeValue = 0.0f;

	/** チャージが最大値に達しているか。 */
	bool bWasFullyCharged = false;
};