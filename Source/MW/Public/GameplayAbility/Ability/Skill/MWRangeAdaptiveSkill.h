#pragma once

#include "Data/Skill/MWSkillTable.h"
#include "GameplayAbility/Ability/Skill/MWSkillBase.h"
#include "MWRangeAdaptiveSkill.generated.h"

class AActor;

UENUM(BlueprintType)
enum class EMWSkillRangeForm : uint8
{
	/** 近距離形態（例：近接コンボ）。 */
	Close,

	/** 遠距離形態（例：飛び道具）。 */
	Far,
};

/**
 * UMWRangeAdaptiveSkill
 *
 * キャスト時に選定したターゲットとの距離に応じて近距離／遠距離形態を選択します。
 */
UCLASS()
class MW_API UMWRangeAdaptiveSkill : public UMWSkillBase
{
	GENERATED_BODY()

protected:
	//~UMWGameplayAbilityインターフェース
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~UMWGameplayAbilityインターフェース終了

private:
	/** SkillIdから距離適応設定を取得します。距離適応型でない場合はnullptrを返します。 */
	const FMWSkillRangeAdaptiveConfig* ResolveRangeAdaptiveConfig(const FMWSkillCastCommand& InCommand) const;

	/** 距離判定に使用する最適なTargetActorを選定します（現在は半径内で最も近いPawn）。 */
	AActor* ResolveBestTargetActor(const FMWSkillRangeAdaptiveConfig& InConfig) const;

	/** 境界付近の揺れを防ぐため、ヒステリシスを用いて近距離／遠距離形態を決定します。 */
	EMWSkillRangeForm DecideRangeForm(float InDistance, const FMWSkillRangeAdaptiveConfig& InConfig);

	/** ターゲットがない場合の形態を決定します。出力Flagでキャスト失敗を要求できます。 */
	EMWSkillRangeForm ResolveNoTargetForm(const FMWSkillRangeAdaptiveConfig& InConfig, bool& bOutShouldFailCast) const;

	/** 決定した形態をCastCommandへ反映します（主にMontageセクションの上書き）。 */
	void ApplyRangeFormToCommand(EMWSkillRangeForm InForm, const FMWSkillRangeAdaptiveConfig& InConfig, FMWSkillCastCommand& InOutCommand) const;

private:
	/** 現在のAbilityライフサイクルで使用するTargetキャッシュ。 */
	UPROPERTY(Transient)
	TObjectPtr<AActor> CachedTargetActor = nullptr;

	/** ヒステリシス判定で使用する直前の形態。 */
	EMWSkillRangeForm LastRangeForm = EMWSkillRangeForm::Far;
};
