// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "MW.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Object.h"
#include "UObject/ScriptMacros.h"
#include "Animation/AnimInstance.h"
#include "PlaySkillAnimCallbackProxy.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMWMontagePlayDelegate, FName, NotifyName);

UCLASS(MinimalAPI)
class UPlaySkillAnimCallbackProxy : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	// Called when Montage finished playing and wasn't interrupted
	UPROPERTY(BlueprintAssignable)
	FOnMWMontagePlayDelegate OnCompleted;

	// Called when Montage starts blending out and is not interrupted
	UPROPERTY(BlueprintAssignable)
	FOnMWMontagePlayDelegate OnBlendOut;

	// Called when Montage has been interrupted (or failed to play)
	UPROPERTY(BlueprintAssignable)
	FOnMWMontagePlayDelegate OnInterrupted;

	UPROPERTY(BlueprintAssignable)
	FOnMWMontagePlayDelegate OnNotifyBegin;

	UPROPERTY(BlueprintAssignable)
	FOnMWMontagePlayDelegate OnNotifyEnd;

	// Called to perform the query internally
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static MW_API UPlaySkillAnimCallbackProxy* CreateProxyObjectForPlaySkillAnim(
		class USkeletalMeshComponent* InSkeletalMeshComponent, 
		class UAnimMontage* MontageToPlay, 
		float PlayRate = 1.f,
		bool bForce = false);

public:
	//~ Begin UObject Interface
	MW_API virtual void BeginDestroy() override;
	//~ End UObject Interface

protected:
	UFUNCTION()
	MW_API void OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	MW_API void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	MW_API void OnNotifyBeginReceived(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);

	UFUNCTION()
	MW_API void OnNotifyEndReceived(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);

private:
	TWeakObjectPtr<UAnimInstance> AnimInstancePtr;
	int32 MontageInstanceID;
	uint32 bInterruptedCalledBeforeBlendingOut : 1;

	bool IsNotifyValid(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload) const;
	void UnbindDelegates();

	FOnMontageBlendingOutStarted BlendingOutDelegate;
	FOnMontageEnded MontageEndedDelegate;

protected:
	// Attempts to play a montage with the specified settings. Returns whether it started or not.
	MW_API bool PlayMontage(
		class USkeletalMeshComponent* InSkeletalMeshComponent,
		class UAnimMontage* MontageToPlay,
		float PlayRate = 1.f,
		bool bForce = false);
};
