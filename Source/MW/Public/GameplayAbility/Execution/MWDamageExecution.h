#pragma once

#include "GameplayEffectExecutionCalculation.h"
#include "MWDamageExecution.generated.h"

/**
 * 基礎Damage、AttackPower、DefensePowerを組み合わせてダメージを算出するExecutionです。
 * ゲーム固有の計算方式に応じて複数のExecutionを実装できます。
 */
UCLASS()
class MW_API UMWDamageExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	// ConstructorとOverride
	UMWDamageExecution();
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

};