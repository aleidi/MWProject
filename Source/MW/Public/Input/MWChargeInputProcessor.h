#pragma once

#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "MWChargeInputProcessor.generated.h"

USTRUCT(BlueprintType)
struct FMWChargeRuntimeConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxChargeValue = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ChargeRate = 70.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DischargeRate = 90.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ChargeStartDelay = 0.15f;
};

USTRUCT()
struct FMWChargeInputState
{
	GENERATED_BODY()

	bool bIsPressed = false;
	bool bIsFullyCharged = false;
	float HoldTime = 0.0f;
	float ChargeValue = 0.0f;

	float RuntimeMaxChargeValue = 100.0f;
	float RuntimeChargeRate = 70.0f;
	float RuntimeDischargeRate = 90.0f;
	float RuntimeChargeStartDelay = 0.15f;
};

UCLASS(BlueprintType)
class MW_API UMWChargeInputProcessor : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MW|Charge", meta = (ClampMin = "0.1"))
	float MaxChargeValue = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MW|Charge", meta = (ClampMin = "0.1"))
	float ChargeRate = 70.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MW|Charge", meta = (ClampMin = "0.1"))
	float DischargeRate = 90.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MW|Charge", meta = (ClampMin = "0.0"))
	float ChargeStartDelay = 0.15f;

public:
	bool IsChargeInputTag(const FGameplayTag& InInputTag) const;
	void HandlePressed(const FGameplayTag& InInputTag, const FMWChargeRuntimeConfig& InRuntimeConfig);

	/** return true means this release belongs to charge flow; cast tag valid means should cast */
	bool HandleReleased(const FGameplayTag& InInputTag, FGameplayTag& OutCastInputTag);

	void Tick(float InDeltaSeconds);
	void Reset();

private:
	FGameplayTag ResolveNormalInputTagFromChargeTag(const FGameplayTag& InChargeInputTag) const;

private:
	UPROPERTY(Transient)
	TMap<FGameplayTag, FMWChargeInputState> ChargeStates;
};
