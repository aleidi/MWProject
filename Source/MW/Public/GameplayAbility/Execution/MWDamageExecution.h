#pragma once

#include "GameplayEffectExecutionCalculation.h"
#include "MWDamageExecution.generated.h"

/**
 * A damage execution, which allows doing damage by combining a raw Damage number with AttackPower and DefensePower
 * Most games will want to implement multiple game-specific executions
 */
UCLASS()
class MW_API UMWDamageExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	// Constructor and overrides
	UMWDamageExecution();
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

};