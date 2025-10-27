// Copyright Epic Games, Inc. All Rights Reserved.

#include "Animation/PlaySkillAnimCallbackProxy.h"
#include "Animation/AnimMontage.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/MWCharacterAnimInstance.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PlaySkillAnimCallbackProxy)

//////////////////////////////////////////////////////////////////////////
// UPlaySkillAnimCallbackProxy

UPlaySkillAnimCallbackProxy::UPlaySkillAnimCallbackProxy(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, MontageInstanceID(INDEX_NONE)
	, bInterruptedCalledBeforeBlendingOut(false)
{
}

UPlaySkillAnimCallbackProxy* UPlaySkillAnimCallbackProxy::CreateProxyObjectForPlaySkillAnim(
	class USkeletalMeshComponent* InSkeletalMeshComponent,
	class UAnimMontage* MontageToPlay,
	float PlayRate,
	bool bForce)
{
	UPlaySkillAnimCallbackProxy* Proxy = NewObject<UPlaySkillAnimCallbackProxy>();
	Proxy->SetFlags(RF_StrongRefOnFrame);
	Proxy->PlayMontage(InSkeletalMeshComponent, MontageToPlay, PlayRate, bForce);
	return Proxy;
}


bool UPlaySkillAnimCallbackProxy::PlayMontage(class USkeletalMeshComponent* InSkeletalMeshComponent, 
	class UAnimMontage* MontageToPlay, 
	float PlayRate,
	bool bForce)
{
	bool bPlayedSuccessfully = false;
	if (InSkeletalMeshComponent)
	{
		if (UMWCharacterAnimInstance* AnimInstance = Cast<UMWCharacterAnimInstance>(InSkeletalMeshComponent->GetAnimInstance()))
		{
			bPlayedSuccessfully = AnimInstance->Montage_Play(MontageToPlay, PlayRate) > 0.f;

			if (bPlayedSuccessfully)
			{
				AnimInstancePtr = AnimInstance;
				if (FAnimMontageInstance* MontageInstance = AnimInstance->GetActiveInstanceForMontage(MontageToPlay))
				{
					MontageInstanceID = MontageInstance->GetInstanceID();
				}

				BlendingOutDelegate.BindUObject(this, &UPlaySkillAnimCallbackProxy::OnMontageBlendingOut);
				AnimInstance->Montage_SetBlendingOutDelegate(BlendingOutDelegate, MontageToPlay);

				MontageEndedDelegate.BindUObject(this, &UPlaySkillAnimCallbackProxy::OnMontageEnded);
				AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, MontageToPlay);

				AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &UPlaySkillAnimCallbackProxy::OnNotifyBeginReceived);
				AnimInstance->OnPlayMontageNotifyEnd.AddDynamic(this, &UPlaySkillAnimCallbackProxy::OnNotifyEndReceived);
			}
		}
	}

	if (!bPlayedSuccessfully)
	{
		OnInterrupted.Broadcast(NAME_None);
	}

	return bPlayedSuccessfully;
}

bool UPlaySkillAnimCallbackProxy::IsNotifyValid(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload) const
{
	return ((MontageInstanceID != INDEX_NONE) && (BranchingPointNotifyPayload.MontageInstanceID == MontageInstanceID));
}


void UPlaySkillAnimCallbackProxy::OnNotifyBeginReceived(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	if (IsNotifyValid(NotifyName, BranchingPointNotifyPayload))
	{
		OnNotifyBegin.Broadcast(NotifyName);
	}
}


void UPlaySkillAnimCallbackProxy::OnNotifyEndReceived(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	if (IsNotifyValid(NotifyName, BranchingPointNotifyPayload))
	{
		OnNotifyEnd.Broadcast(NotifyName);
	}
}


void UPlaySkillAnimCallbackProxy::OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
	if (bInterrupted)
	{
		OnInterrupted.Broadcast(NAME_None);
		bInterruptedCalledBeforeBlendingOut = true;
	}
	else
	{
		OnBlendOut.Broadcast(NAME_None);
	}
}

void UPlaySkillAnimCallbackProxy::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!bInterrupted)
	{
		OnCompleted.Broadcast(NAME_None);
	}
	else if (!bInterruptedCalledBeforeBlendingOut)
	{
		OnInterrupted.Broadcast(NAME_None);
	}

	UnbindDelegates();
}

void UPlaySkillAnimCallbackProxy::UnbindDelegates()
{
	if (UAnimInstance* AnimInstance = AnimInstancePtr.Get())
	{
		AnimInstance->OnPlayMontageNotifyBegin.RemoveDynamic(this, &UPlaySkillAnimCallbackProxy::OnNotifyBeginReceived);
		AnimInstance->OnPlayMontageNotifyEnd.RemoveDynamic(this, &UPlaySkillAnimCallbackProxy::OnNotifyEndReceived);
	}
}

void UPlaySkillAnimCallbackProxy::BeginDestroy()
{
	UnbindDelegates();

	Super::BeginDestroy();
}
