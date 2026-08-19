#include "GameplayAbility/Execution/MWDamageExecution.h"
#include "GameplayAbility/Attribute/MWBattleAttributeSet.h"
#include "GameplayAbility/Execution/MWCalculationTable.h"

struct MWDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Endurance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Strength);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);

	MWDamageStatics()
	{
		// 実行適用時の値を使用するため、TargetのEndurance属性をスナップショットせずにキャプチャ
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMWBattleAttributeSet, Endurance, Target, false);

		// ダメージ実行用GameplayEffectSpecの作成時点でSourceのStrengthをスナップショット
		// これにより、命中時ではなく発射時のStrengthを基準にダメージを計算する。
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMWBattleAttributeSet, Strength, Source, true);

		// 通常はExecution経由で直接渡されるSourceの基礎Damageもキャプチャ
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMWBattleAttributeSet, Damage, Source, true);
	}
};

static const MWDamageStatics& DamageStatics()
{
	static MWDamageStatics DmgStatics;
	return DmgStatics;
}

UMWDamageExecution::UMWDamageExecution()
{
	// 属性をキャプチャ
	RelevantAttributesToCapture.Add(DamageStatics().EnduranceDef);
	RelevantAttributesToCapture.Add(DamageStatics().StrengthDef);
	RelevantAttributesToCapture.Add(DamageStatics().DamageDef);
}

void UMWDamageExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	// 適用するバフの判定に使用するSourceとTargetのタグを取得
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float def = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().EnduranceDef, EvaluationParameters, def);

	float atk = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().StrengthDef, EvaluationParameters, atk);

	const float dmg = MWCalculationTable::DamageCalulation(atk, def);
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().DamageProperty, EGameplayModOp::Additive, dmg));
}
