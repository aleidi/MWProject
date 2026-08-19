#pragma once

#include "MW.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_ChargeTick.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMWOnChargeValueChanged, float, NewValue, float, Percent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMWOnChargeFull);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMWOnChargeZero);

/**
 * チャージ／減衰をフレームごとに更新するTask。
 */
UCLASS()
class MW_API UAbilityTask_ChargeTick : public UAbilityTask
{
	GENERATED_BODY()

public:
	UAbilityTask_ChargeTick();

	/** チャージTaskを生成して開始します。 */
	UFUNCTION(BlueprintCallable, Category="MW|Charge", meta=(DefaultToSelf="OwningAbility", HidePin="OwningAbility", BlueprintInternalUseOnly="true"))
	static UAbilityTask_ChargeTick* StartChargeTask(
		UGameplayAbility* OwningAbility,
		float InMaxCharge = 100.f,
		float InChargePerSec = 30.f,
		float InDecayPerSec = 50.f);

	/** チャージ（true）または減衰（false）へ切り替えます。 */
	UFUNCTION(BlueprintCallable, Category="MW|Charge")
	void SetCharging(bool bCharging);

	/** 現在のチャージ値。 */
	UFUNCTION(BlueprintPure, Category="MW|Charge")
	float GetCurrentCharge() const { return CurrentCharge; }

	UFUNCTION(BlueprintCallable, Category="MW|Charge")
	void SetTickEnabled(bool bEnabled);

	UFUNCTION(BlueprintCallable, Category="MW|Charge")
	void SetMaxCharge(float InMaxCharge);

	UFUNCTION(BlueprintCallable, Category="MW|Charge")
	void SetChargePerSec(float InChargePerSec);

	UFUNCTION(BlueprintCallable, Category="MW|Charge")
	void SetDecayPerSec(float InDecayPerSec);

public:
	UPROPERTY(BlueprintAssignable)
	FMWOnChargeValueChanged OnValueChanged;

	UPROPERTY(BlueprintAssignable)
	FMWOnChargeFull OnFull;

	UPROPERTY(BlueprintAssignable)
	FMWOnChargeZero OnZero;

protected:
	virtual void Activate() override;
	virtual void TickTask(float DeltaTime) override;
	virtual void OnDestroy(bool bInOwnerFinished) override;

private:
	float MaxCharge = 100.f;
	float ChargePerSec = 30.f;
	float DecayPerSec = 50.f;

	float CurrentCharge = 0.f;
	bool bIsCharging = false;
	bool bWasFull = false;
	bool bWasZero = true;

	bool bDoTickTask = false;
};