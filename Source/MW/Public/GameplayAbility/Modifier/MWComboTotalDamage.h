#pragma once

#include "GameplayModMagnitudeCalculation.h"
#include "MWComboTotalDamage.generated.h"

/**
 * 基礎Damage、AttackPower、DefensePowerを組み合わせてダメージを算出するExecutionです。
 * ゲーム固有の計算方式に応じて複数のExecutionを実装できます。
 */
UCLASS()
class MW_API UMWComboTotalDamage : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	// ConstructorとOverride
	UMWComboTotalDamage();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
};