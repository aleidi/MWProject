#pragma once

// Include Header
#include "GameplayAbility/Ability/Battle/MWBattleMoveCommand.h"
#include "Define/MWDefineBattle.h"
#include "Define/MWDefineGameplay.h"
#include "MWBattleUnitAttack.generated.h"

// Forward Declare
struct FGameplayEventData;
class UGameplayEffect;
// Define

/*
 * @class UMWBattleUnitAttack
 * 
 * @brief UMWBattleUnitAttack processes the attack command of battle unit.
 *        UMWBattleUnitAttackは、戦闘ユニットの攻撃コマンドを処理する。
 *        　〇   ーーー   〇   ーーー   〇  ーーー   〇
 *                 |           |           |
 *		  ↑↓＋〇 ーーー ↑↓＋〇 ーーー ↑↓＋〇 ーーー ↑↓＋〇
 * 　　　　         |           |           |
 * 　　　　←→＋〇 ーーー ←→＋〇 ーーー ←→＋〇 ーーー ←→＋〇
 *        The attack combo is like the above diagram.
 *        攻撃コンボは上記の図のようになる。
 *        Get the skill animation from the character's skill table based on the current combo step and play it.
 *        キャラクターのスキルテーブルから現在のコンボステップに基づいてスキルアニメーションを取得して、再生する。
 * 
 * @note
 */
UCLASS()
class UMWBattleUnitAttack : public UMWBattleMoveCommand
{
	GENERATED_BODY()

public:
	UMWBattleUnitAttack(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	void OnCombo(const FGameplayEventData* Payload);

	void OnComboHit(const FGameplayEventData* Payload);

	void OnComboEnd();

	void OnAvatarChange(const FGameplayEventData* Payload);

	void BindDelegates();

	void UnbindDelegates();

	UFUNCTION()
	void OnSkillAnimBlendingOut(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void OnSkillAnimEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void OnApproachAnimBlendingOut(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void OnApproachAnimEnded(UAnimMontage* Montage, bool bInterrupted);
	
	UFUNCTION()
	void OnReturnAnimBlendingOut(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void OnReturnAnimEnded(UAnimMontage* Montage, bool bInterrupted);

	void SetupSkillTable(AActor* AvatarActor);

	// Reset combo state
	// コンボ状態をリセットする
	void ResetCombo();

	// Play montage with blending out and end delegates.
	// ブレンディングアウトと終了デリゲートを設定してモンタージュを再生する.
	bool PlayMontage(UAnimMontage* InMontage, FOnMontageBlendingOutStarted& BlendingOutDelegate, FOnMontageEnded& EndDelegate);

	// Play skill animation montage.
	// スキルアニメーションモンタージュを再生する.
	bool PlaySkillAnim(UAnimMontage* InAnimation);

	// Check if currently doing approach.
	// 今接近中か確認する.
	bool IsDoingApproach() const;

	// Set approach state for the current combo step.
	// 現在のコンボステップのアプローチ状態を設定する.
	void SetApproachState(int32 CurrentComboStep, bool NewSetate);

	void ResetApproachStates();

	// Check if the original position flag is set.
	// 元の位置フラグが設定されているかどうかを確認する.
	bool IsOriginalPositionFlagSet() const;

	void SetOriginalPositionFlag(bool NewSetate);

	void CalculateComboDamage();

	void CalcualteComboHitNum(UAnimSequenceBase* InAnim);

	void ApplyComboTotalDamageToSource();

	void ApplyComboDamageToTarget(const AActor* InTargetActor, float ComboHitRatio);

protected:
	// skills table
	// スキルテーブル
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Battle|UnitAttack")
	FMWCharacterBattleSkillTable SkillTable;

	// The tag to receive combo event after ability activated.
	// アビリティ発動後にコンボイベントを受信するタグ
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Battle|UnitAttack")
	FGameplayTag ComboTag;

	// The tag to receive combo event after ability activated.
	// アビリティ発動後にコンボイベントを受信するタグ
	UPROPERTY(BlueprintReadOnly, Category = "Battle|UnitAttack")
	FGameplayTag ComboHitTag = MWGameplayTags::GP_Battle_ComboHit;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Battle|UnitAttack")
	FGameplayTag AvatarChangeTag;

	// It's used to determine whether to return to the original position after approaching.
	// If current position is far from the original position more than this distance, return to the original position.
	// 接近後に元の位置に戻るかどうかを判断するために使用されます。
	// 現在の位置がこの距離よりも元の位置から遠い場合、元の位置に戻ります。
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Battle|UnitAttack")
	float MaxDistanceFromInitialPosition = 0.1f;

	TObjectPtr<const AActor> AttackTargetActor = nullptr;

	UPROPERTY(EditAnywhere, Category = "Battle|UnitAttack")
	TSubclassOf<UGameplayEffect> GE_ComboTotalDamage;

	UPROPERTY(EditAnywhere, Category = "Battle|UnitAttack")
	TSubclassOf<UGameplayEffect> GE_ComboHit;

	// Current Combo Index
	// 現在のコンボインデックス
	int32 CurrComboStep = 0;

	int32 CurrComboTotalDmg = 0;

	int32 ComboHitNum = 0;

	int32 CurrComboHitIdx = 0;

	EMWCharacterSkillComboType CurrComboType;

	EMWAttackResult CurrComboResult = EMWAttackResult::Max;

	// Approach states for each combo step.
	// 各コンボステップのアプローチ状態。
	TMap<int32, bool> ApproachStates;

	bool bIsOriginalPositionSet = false;

	FVector OriginalPosition;

	FDelegateHandle ComboEventHandle;

	FDelegateHandle ComboHitEventHandle;

	FDelegateHandle AvatarChangeEventHandle;

	FOnMontageBlendingOutStarted SkillAnimBlendingOutDelegate;

	FOnMontageEnded SkillAnimEndedDelegate;

	FOnMontageBlendingOutStarted ApproachAnimBlendingOutDelegate;

	FOnMontageEnded ApproachAnimEndedDelegate;

	FOnMontageBlendingOutStarted ReturnAnimBlendingOutDelegate;

	FOnMontageEnded ReturnAnimEndedDelegate;
};