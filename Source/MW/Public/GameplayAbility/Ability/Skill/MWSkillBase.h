#pragma once

#include "GameplayAbility/MWGameplayAbility.h"
#include "MWSkillBase.generated.h"

class UAbilityTask_PlayMontageAndWait;
class UAnimMontage;

/**
 * UMWSkillBase
 * 
 * 按键释放能力技能
 */
UCLASS()
class MW_API UMWSkillBase : public UMWGameplayAbility
{
	GENERATED_BODY()

public:
	UMWSkillBase(const FObjectInitializer& ObjectInitializer);

protected:
	//~UMWGameplayAbility interface
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~End of UMWGameplayAbility interface

	void ClearMontageTask();

	/**
	 * 检查当前是否可以播放技能动画。
	 * 当角色没有任何 Montage 正在播放时，直接返回 true；
	 * 若有 Montage 正在播放，则检查 AbilitySystemComponent 是否带有
	 * Ability.Skill.Chainable 标签（连携窗口），有则允许播放，否则返回 false。
	 * TODO:根据后续的实现可能会有所修改
	 */
	virtual bool CanPlayAbilityAnimation() const;

	void PlayAbilityAnimation();

	UFUNCTION()
	virtual void OnMontageCompleted();

	UFUNCTION()
	virtual void OnMontageCancelled();

	UFUNCTION()
	virtual void OnMontageInterrupted();

protected:
	/** 技能动画 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MW|Animation")
	TObjectPtr<UAnimMontage> AbilityAnim = nullptr;

	/** Montage 播放速率 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MW|Animation", meta = (ClampMin = "0.1"))
	float MontagePlayRate = 1.0f;

	/** Montage 起始段名称，为 None 则从头播放 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MW|Animation")
	FName MontageSectionName = NAME_None;

private:
	UPROPERTY()
	TObjectPtr<UAbilityTask_PlayMontageAndWait> MontageTask = nullptr;
};