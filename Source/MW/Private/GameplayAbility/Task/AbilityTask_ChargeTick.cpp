#include "GameplayAbility/Task/AbilityTask_ChargeTick.h"

#define DEBUG_PRINT(Time, Format, ...) \
	if (GEngine) \
	{ \
		GEngine->AddOnScreenDebugMessage(-1, Time, FColor::Yellow, FString::Printf(TEXT(Format), ##__VA_ARGS__)); \
	}

UAbilityTask_ChargeTick::UAbilityTask_ChargeTick()
{
	bTickingTask = true;
}

UAbilityTask_ChargeTick* UAbilityTask_ChargeTick::StartChargeTask(
	UGameplayAbility* OwningAbility,
	float InMaxCharge,
	float InChargePerSec,
	float InDecayPerSec)
{
	UAbilityTask_ChargeTick* Task = NewAbilityTask<UAbilityTask_ChargeTick>(OwningAbility);
	Task->MaxCharge = FMath::Max(0.f, InMaxCharge);
	Task->ChargePerSec = FMath::Max(0.f, InChargePerSec);
	Task->DecayPerSec = FMath::Max(0.f, InDecayPerSec);
	return Task;
}

void UAbilityTask_ChargeTick::Activate()
{
	SetTickEnabled(true);
}

void UAbilityTask_ChargeTick::SetCharging(bool bCharging)
{
	bIsCharging = bCharging;
}

void UAbilityTask_ChargeTick::SetTickEnabled(bool bEnabled)
{
	bDoTickTask = bEnabled;
}

void UAbilityTask_ChargeTick::SetMaxCharge(float InMaxCharge)
{
	MaxCharge = FMath::Max(0.0f, InMaxCharge);
}

void UAbilityTask_ChargeTick::SetChargePerSec(float InChargePerSec)
{
	ChargePerSec = FMath::Max(0.f, InChargePerSec);
}

void UAbilityTask_ChargeTick::SetDecayPerSec(float InDecayPerSec)
{
	DecayPerSec = FMath::Max(0.f, InDecayPerSec);
}

void UAbilityTask_ChargeTick::TickTask(float DeltaTime)
{
	if (!bDoTickTask)
	{
		return;
	}

	const float Step = DeltaTime;
	const float Old = CurrentCharge;

	if (bIsCharging)
	{
		CurrentCharge = FMath::Min(CurrentCharge + ChargePerSec * Step, MaxCharge);
	}
	else
	{
		CurrentCharge = FMath::Max(CurrentCharge - DecayPerSec * Step, 0.f);
	}

	const float Percent = MaxCharge > 0.f ? (CurrentCharge / MaxCharge) : 0.f;

	if (!FMath::IsNearlyEqual(Old, CurrentCharge))
	{
		OnValueChanged.Broadcast(CurrentCharge, Percent);
	}

	// 满值事件（只触发一次直到不再满）
	const bool bNowFull = FMath::IsNearlyEqual(CurrentCharge, MaxCharge);
	if (bNowFull && !bWasFull)
	{
		bWasFull = true;
		OnFull.Broadcast();
		DEBUG_PRINT(2.f, "Charge Full!");
	}
	else if (!bNowFull)
	{
		bWasFull = false;
	}

	// 归零事件（只触发一次直到不再为零）
	const bool bNowZero = FMath::IsNearlyZero(CurrentCharge);
	if (bNowZero && !bWasZero)
	{
		bWasZero = true;
		OnZero.Broadcast();
		DEBUG_PRINT(2.f, "Charge Depleted!");
	}
	else if (!bNowZero)
	{
		bWasZero = false;
	}
}

void UAbilityTask_ChargeTick::OnDestroy(bool bInOwnerFinished)
{
	Super::OnDestroy(bInOwnerFinished);
}