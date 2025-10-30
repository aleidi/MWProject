#pragma once

// Include Header
#include "GameplayAbility/Ability/Battle/MWBattleMoveCommand.h"
#include "Define/MWDefineGameplay.h"
#include "MWBattleUnitAttack.generated.h"

// Forward Declare
struct FGameplayEventData;

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

	void OnComboEnd();

	void OnAvatarChange(const FGameplayEventData* Payload);

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

	void ResetCombo();

	void BindDelegates();

	void UnbindDelegates();

	bool PlayMontage(UAnimMontage* InMontage, FOnMontageBlendingOutStarted& BlendingOutDelegate, FOnMontageEnded& EndDelegate);

	bool IsDoingApproach() const;

	void SetApproachState(int32 CurrentComboStep, bool NewSetate);

	void ResetApproachStates();

	bool IsOriginalPositionFlagSet() const;

	void SetOriginalPositionFlag(bool NewSetate);

	bool PlaySkillAnim(UAnimMontage* InAnimation);

protected:
	// skills table
	// スキルテーブル
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Battle|UnitAttack")
	FMWCharacterSkillTable SkillTable;

	// The tag to receive combo event after ability activated.
	// アビリティ発動後にコンボイベントを受信するタグ
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Battle|UnitAttack")
	FGameplayTag ComboTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Battle|UnitAttack")
	FGameplayTag AvatarChangeTag;

	// It's used to determine whether to return to the original position after approaching.
	// If current position is far from the original position more than this distance, return to the original position.
	// 接近後に元の位置に戻るかどうかを判断するために使用されます。
	// 現在の位置がこの距離よりも元の位置から遠い場合、元の位置に戻ります。
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Battle|UnitAttack")
	float MaxDistanceFromInitialPosition = 0.1f;

	// Current Combo Index
	// 現在のコンボインデックス
	UPROPERTY(BlueprintReadOnly)
	int32 ComboStep = 0;

	UPROPERTY(BlueprintReadOnly)
	EMWCharacterSkillComboType ComboType;

	// Approach states for each combo step.
	// 各コンボステップのアプローチ状態。
	UPROPERTY()
	TMap<int32, bool> ApproachStates;

	UPROPERTY()
	bool bIsOriginalPositionSet = false;

	UPROPERTY()
	FVector OriginalPosition;

	FDelegateHandle ComboEventHandle;

	FDelegateHandle AvatarChangeEventHandle;

	FOnMontageBlendingOutStarted SkillAnimBlendingOutDelegate;

	FOnMontageEnded SkillAnimEndedDelegate;

	FOnMontageBlendingOutStarted ApproachAnimBlendingOutDelegate;

	FOnMontageEnded ApproachAnimEndedDelegate;

	FOnMontageBlendingOutStarted ReturnAnimBlendingOutDelegate;

	FOnMontageEnded ReturnAnimEndedDelegate;
};