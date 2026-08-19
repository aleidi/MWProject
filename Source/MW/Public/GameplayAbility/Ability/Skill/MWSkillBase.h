#pragma once

#include "GameplayAbility/MWGameplayAbility.h"
#include "GameplayAbility/Ability/Skill/MWSkillCastTypes.h"
#include "MWSkillBase.generated.h"

class UAbilityTask_PlayMontageAndWait;
class UAnimMontage;

/**
 * UMWSkillBase
 * 
 * ボタンを離したときに発動するアビリティスキル。
 */
UCLASS()
class MW_API UMWSkillBase : public UMWGameplayAbility
{
	GENERATED_BODY()

protected:
	//~UMWGameplayAbilityインターフェース
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual bool CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;
	//~UMWGameplayAbilityインターフェース終了

	void ClearMontageTask();

	/**
	 * 現在スキルアニメーションを再生できるか確認します。
	 * キャラクターがMontageを再生していない場合はtrueを返します。
	 * Montageの再生中は、AbilitySystemComponentがAbility.Skill.Chainableタグ
	 * （連携可能期間）を持つ場合のみ再生を許可します。
	 */
	virtual bool CanPlayAbilityAnimation() const;

	void PlayAbilityAnimation();

	UFUNCTION()
	virtual void OnMontageCompleted();

	UFUNCTION()
	virtual void OnMontageCancelled();

	UFUNCTION()
	virtual void OnMontageInterrupted();

	bool TryResolveCastCommand(const FGameplayEventData* TriggerEventData, FMWSkillCastCommand& OutCommand) const;

	bool TryResolveSkillPresentation(const FMWSkillCastCommand& InCommand);

	void SetPendingCommitSkillId(int32 InSkillId);

	bool TryCommitAndPlayFromCommand(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FMWSkillCastCommand& InCommand);

protected:
	/** スキルアニメーション。 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MW|Animation")
	TObjectPtr<UAnimMontage> AbilityAnim = nullptr;

	/** Montageの再生速度。 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MW|Animation", meta = (ClampMin = "0.1"))
	float MontagePlayRate = 1.0f;

	/** Montageの開始セクション名。Noneの場合は先頭から再生します。 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MW|Animation")
	FName MontageSectionName = NAME_None;

private:
	UPROPERTY()
	TObjectPtr<UAbilityTask_PlayMontageAndWait> MontageTask = nullptr;

	UPROPERTY(Transient)
	int32 PendingCommitSkillId = INDEX_NONE;
};
