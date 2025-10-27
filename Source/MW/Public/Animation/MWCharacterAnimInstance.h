#pragma once

#include "Animation/AnimInstance.h"
#include "Define/MWDefineGameplay.h"
#include "Interface/MWCharacterAnimInterface.h"
#include "MWCharacterAnimInstance.generated.h"



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

protected:
	UPROPERTY(BlueprintReadOnly, Category="Anim")
	TEnumAsByte<ECharacterBehaviorState> BehaviorState;
	UPROPERTY(BlueprintReadOnly, Category="Anim")
	bool bIsMoving;
	UPROPERTY(BlueprintReadOnly, Category="Anim")
	bool bCanNormalAtk;
	UPROPERTY(BlueprintReadOnly, Category="Anim")
	bool bCanCastSkill;
};