#include "Character/MWCharacterAnimInstance.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Gameplay/MWGameplayTags.h"

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