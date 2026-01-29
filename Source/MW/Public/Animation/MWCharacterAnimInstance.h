#pragma once

#include "Animation/AnimInstance.h"
#include "Define/MWDefineGameplay.h"
#include "Interface/MWAbilityInputInterface.h"
#include "MWCharacterAnimInstance.generated.h"

struct FGameplayTag;

UCLASS()
class UMWCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	virtual void NativeBeginPlay() override;

private:
	UFUNCTION()
	void ProcessOnMontageStarted(UAnimMontage* MontageInst);
	UFUNCTION()
	void ProcessOnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

	void OnBattleStateTagChanged(const FGameplayTag Tag, int32 NewCount);
		
protected:
	UPROPERTY(BlueprintReadOnly, Category="Anim")
	TEnumAsByte<ECharacterBehaviorState> BehaviorState;
	UPROPERTY(BlueprintReadOnly, Category="Anim")
	bool bIsMoving;
	UPROPERTY(BlueprintReadOnly, Category="Anim")
	bool bCanNormalAtk;
	UPROPERTY(BlueprintReadOnly, Category="Anim")
	bool bCanCastSkill;

	FDelegateHandle BattleStateTagHandle;
};