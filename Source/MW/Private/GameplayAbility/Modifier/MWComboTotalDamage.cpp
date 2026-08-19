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
		// 実行適用時の値を使用するため、TargetのEndurance属性をスナップショットせずにキャプチャ
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMWBattleAttributeSet, Endurance, Target, false);

		// ダメージ実行用GameplayEffectSpecの作成時点でSourceのStrengthをスナップショット
		// これにより、命中時ではなく発射時のStrengthを基準にダメージを計算する。
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMWBattleAttributeSet, Strength, Source, true);

		// 通常はExecution経由で直接渡されるSourceの基礎ComboTotalDamageもキャプチャ
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
	// 属性をキャプチャ
	RelevantAttributesToCapture.Add(ComboTotalDamageStatics().EnduranceDef);
	RelevantAttributesToCapture.Add(ComboTotalDamageStatics().StrengthDef);
	RelevantAttributesToCapture.Add(ComboTotalDamageStatics().ComboTotalDamageDef);
}

float UMWComboTotalDamage::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	return 9999.f;
}