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
	// Montageが中断されずに再生を完了した際に呼び出されます。
	UPROPERTY(BlueprintAssignable)
	FOnMWMontagePlayDelegate OnCompleted;

	// Montageが中断されずにブレンドアウトを開始した際に呼び出されます。
	UPROPERTY(BlueprintAssignable)
	FOnMWMontagePlayDelegate OnBlendOut;

	// Montageが中断された、または再生に失敗した際に呼び出されます。
	UPROPERTY(BlueprintAssignable)
	FOnMWMontagePlayDelegate OnInterrupted;

	UPROPERTY(BlueprintAssignable)
	FOnMWMontagePlayDelegate OnNotifyBegin;

	UPROPERTY(BlueprintAssignable)
	FOnMWMontagePlayDelegate OnNotifyEnd;

	// 内部処理を実行します。
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static MW_API UPlaySkillAnimCallbackProxy* CreateProxyObjectForPlaySkillAnim(
		class USkeletalMeshComponent* InSkeletalMeshComponent, 
		class UAnimMontage* MontageToPlay, 
		float PlayRate = 1.f,
		bool bForce = false);

public:
	//~ UObjectインターフェース開始
	MW_API virtual void BeginDestroy() override;
	//~ UObjectインターフェース終了

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
	// 指定した設定でMontageの再生を試行し、開始できたかを返します。
	MW_API bool PlayMontage(
		class USkeletalMeshComponent* InSkeletalMeshComponent,
		class UAnimMontage* MontageToPlay,
		float PlayRate = 1.f,
		bool bForce = false);
};
