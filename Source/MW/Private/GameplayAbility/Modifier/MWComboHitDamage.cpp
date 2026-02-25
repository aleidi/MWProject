#include "GameplayAbility/Modifier/MWComboHitDamage.h"

#include "GameplayEffectExecutionCalculation.h"
#include "GameplayAbility/Attribute/MWBattleAttributeSet.h"
#include "GameplayAbility/Execution/MWCalculationTable.h"
#include "Gameplay/MWGameplayTags.h"

struct MWComboHitDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(ComboTotalDamage);

	MWComboHitDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMWBattleAttributeSet, ComboTotalDamage, Source, true);
	}
};

static const MWComboHitDamageStatics& ComboHitDamageStatics()
{
	static MWComboHitDamageStatics DmgStatics;
	return DmgStatics;
}

UMWComboHitDamage::UMWComboHitDamage()
{
	RelevantAttributesToCapture.Add(ComboHitDamageStatics().ComboTotalDamageDef);
}

float UMWComboHitDamage::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather the tags from the source and target as that can affect which buffs should be used
	// ソースとターゲットからタグを収集します。これにより、使用するバフに影響を与える可能性があります.
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float sourceComboTotalDamage = 0.f;
	GetCapturedAttributeMagnitude(ComboHitDamageStatics().ComboTotalDamageDef, Spec, EvaluationParameters, sourceComboTotalDamage);

	const float comboHitRatio = Spec.GetSetByCallerMagnitude(MWGameplayTags::GP_SetByCaller_ComboHitRatio, false, 1.0f);

	// Use minus value to subtract damage from health for add op.
	// 体力からダメージを引くためにマイナス値を使用します.
	const float result = sourceComboTotalDamage * comboHitRatio * -1.f;

	return FMath::Floor(result);
}