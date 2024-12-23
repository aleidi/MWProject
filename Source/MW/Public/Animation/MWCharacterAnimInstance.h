#pragma once

#include "Animation/AnimInstance.h"
#include "Character/MWCharacterTypes.h"
#include "Interface/MWCharacterAnimInterface.h"
#include "MWCharacterAnimInstance.generated.h"



UCLASS()
class UMWCharacterAnimInstance : public UAnimInstance,
								public IMWCharacterAnimInterface
{
	GENERATED_BODY()

protected:
	virtual void NativeBeginPlay() override;

public:
	virtual bool CastSkillAnim(UAnimMontage* MontageToPlay, float PlayRate, bool bForce) override;

	/* Play a skill animation. */
	UFUNCTION(BlueprintCallable, Category = "Animation|Montage")
	bool PlaySkillAnimation(UAnimMontage* MontageToPlay, float InPlayRate = 1.f, bool bForce = false);

private:
	UFUNCTION()
	void ProcessOnMontageStarted(UAnimMontage* MontageInst);
	UFUNCTION()
	void ProcessOnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

	/* Disallow to play a montage. */
	FORCEINLINE void DisableSkillAnim() { bCanCastSkill = false; }
	/* Allow to play a montage. */
	FORCEINLINE void EnableSkillAnim() { bCanCastSkill = true; }
	/* Check if can play skill. */
	FORCEINLINE bool CanPlaySkillAnim() const { return bCanCastSkill; }

protected:
	UPROPERTY(BlueprintReadOnly, Category="Anim")
	TEnumAsByte<MWBehaviorState::EBehaviorState> BehaviorState;
	UPROPERTY(BlueprintReadOnly, Category="Anim")
	bool bIsMoving;
	UPROPERTY(BlueprintReadOnly, Category="Anim")
	bool bCanNormalAtk;
	UPROPERTY(BlueprintReadOnly, Category="Anim")
	bool bCanCastSkill;

	UPROPERTY()
	TObjectPtr<UAnimMontage> PlayingMontageRef;

	static FName EnableSkillAnimNotifyName;
	static FName DisableSkillAnimNotifyName;
};