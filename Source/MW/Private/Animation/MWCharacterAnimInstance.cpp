#include "Animation/MWCharacterAnimInstance.h"

void UMWCharacterAnimInstance::NativeBeginPlay()
{
	OnMontageStarted.AddDynamic(this, &UMWCharacterAnimInstance::ProcessOnMontageStarted);
	OnPlayMontageNotifyBegin.AddDynamic(this, &UMWCharacterAnimInstance::ProcessOnMontageNotifyBegin);
}

void UMWCharacterAnimInstance::ProcessOnMontageStarted(UAnimMontage* MontageInst)
{
}

void UMWCharacterAnimInstance::ProcessOnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
}
