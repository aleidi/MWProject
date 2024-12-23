#include "Animation/MWCharacterAnimInstance.h"

FName UMWCharacterAnimInstance::EnableSkillAnimNotifyName = TEXT("EnableSkillAnimNotify");
FName UMWCharacterAnimInstance::DisableSkillAnimNotifyName = TEXT("DisableSkillAnimNotify");

void UMWCharacterAnimInstance::NativeBeginPlay()
{
	OnMontageStarted.AddDynamic(this, &UMWCharacterAnimInstance::ProcessOnMontageStarted);
	OnPlayMontageNotifyBegin.AddDynamic(this, &UMWCharacterAnimInstance::ProcessOnMontageNotifyBegin);


#if WITH_EDITOR
	EnableSkillAnim();
	bCanNormalAtk = true;
#endif
}

bool UMWCharacterAnimInstance::CastSkillAnim(UAnimMontage* MontageToPlay, float PlayRate, bool bForce)
{
	return PlaySkillAnimation(MontageToPlay, PlayRate, bForce);
}

bool UMWCharacterAnimInstance::PlaySkillAnimation(UAnimMontage* MontageToPlay, float InPlayRate, bool bForce)
{
	if (CanPlaySkillAnim() || bForce)
	{
		return Montage_Play(MontageToPlay, InPlayRate) > 0.0f;
	}

	return false;
}

void UMWCharacterAnimInstance::ProcessOnMontageStarted(UAnimMontage* MontageInst)
{
	DisableSkillAnim();
}

void UMWCharacterAnimInstance::ProcessOnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	// enable skill montage Play
	if (NotifyName == EnableSkillAnimNotifyName)
	{
		EnableSkillAnim();
	}

	// disable skill montage play
	if (NotifyName == DisableSkillAnimNotifyName)
	{
		DisableSkillAnim();
	}
}
