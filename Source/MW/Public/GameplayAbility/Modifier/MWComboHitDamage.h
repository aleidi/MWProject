#pragma once

#include "GameplayModMagnitudeCalculation.h"
#include "MWComboHitDamage.generated.h"

/**
 * A damage execution, which allows doing damage by combining a raw Damage number with AttackPower and DefensePower
 * Most games will want to implement multiple game-specific executions
 */
UCLASS()
class MW_API UMWComboHitDamage : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	// Constructor and overrides
	UMWComboHitDamage();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
};