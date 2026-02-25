#include "GameplayAbility/Modifier/MWComboTotalDamage.h"

#include "GameplayEffectExecutionCalculation.h"
#include "GameplayAbility/Attribute/MWBattleAttributeSet.h"
#include "GameplayAbility/Execution/MWCalculationTable.h"

struct MWComboTotalDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Endurance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Strength);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ComboTotalDamage);

	MWComboTotalDamageStatics()
	{
		// Capture the Target's Endurance attribute. Do not snapshot it, because we want to use the health value at the moment we apply the execution.
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMWBattleAttributeSet, Endurance, Target, false);

		// Capture the Source's Strength. We do want to snapshot this at the moment we create the GameplayEffectSpec that will execute the damage.
		// (imagine we fire a projectile: we create the GE Spec when the projectile is fired. When it hits the target, we want to use the Strength at the moment
		// the projectile was launched, not when it hits).
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMWBattleAttributeSet, Strength, Source, true);

		// Also capture the source's raw ComboTotalDamage, which is normally passed in directly via the execution
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMWBattleAttributeSet, ComboTotalDamage, Source, true);
	}
};

static const MWComboTotalDamageStatics& ComboTotalDamageStatics()
{
	static MWComboTotalDamageStatics DmgStatics;
	return DmgStatics;
}

UMWComboTotalDamage::UMWComboTotalDamage()
{
	// Capture attribute here
	RelevantAttributesToCapture.Add(ComboTotalDamageStatics().EnduranceDef);
	RelevantAttributesToCapture.Add(ComboTotalDamageStatics().StrengthDef);
	RelevantAttributesToCapture.Add(ComboTotalDamageStatics().ComboTotalDamageDef);
}

float UMWComboTotalDamage::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	return 9999.f;
}