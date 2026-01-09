// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameplayAbility/MWGameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "NativeGameplayTags.h"

#include "MWAbilitySystemComponent.generated.h"

class AActor;
class UGameplayAbility;
class UMWAbilityTagRelationshipMapping;
class UObject;
struct FFrame;
struct FGameplayAbilityTargetDataHandle;
struct FInputActionValue;
struct FInputActionInstance;

MW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_AbilityInputBlocked);

USTRUCT()
struct FMWAbilityInputActionPayload 
{
	GENERATED_BODY()

	UPROPERTY()
	FInputActionValue Value;

	// Total trigger processing/evaluation time (How long this action has been in event Started, Ongoing, or Triggered
	UPROPERTY()
	float ElapsedProcessedTime = 0.f;

	// Triggered time (How long this action has been in event Triggered only)
	UPROPERTY()
	float ElapsedTriggeredTime = 0.f;
};

USTRUCT()
struct FMWAbilityReleasedInput
{
	GENERATED_BODY()

	UPROPERTY()
	FGameplayAbilitySpecHandle SpecHandle;

	UPROPERTY()
	FMWAbilityInputActionPayload Payload;

	FORCEINLINE bool operator==(const FMWAbilityReleasedInput& Other) const
	{
		return SpecHandle == Other.SpecHandle;
	}
};

/**
 * UMWAbilitySystemComponent
 *
 *	Base ability system component class used by this project.
 */
UCLASS()
class MW_API UMWAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UMWAbilitySystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~UActorComponent interface
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~End of UActorComponent interface

	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;

	typedef TFunctionRef<bool(const UMWGameplayAbility* MWAbility, FGameplayAbilitySpecHandle Handle)> TShouldCancelAbilityFunc;
	void CancelAbilitiesByFunc(TShouldCancelAbilityFunc ShouldCancelFunc, bool bReplicateCancelAbility);

	void CancelInputActivatedAbilities(bool bReplicateCancelAbility);

	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FInputActionInstance& ActionInst, const FGameplayTag& InputTag);

	/* Help function to call AbilityInputTagPressed of Actor. */
	static bool ActorAbilityInputTagPressed(AActor* InActor, const FGameplayTag& InputTag);
	/* Help function to call AbilityInputTagReleased of Actor. */
	static bool ActorAbilityInputTagReleased(AActor* InActor, const FGameplayTag& InputTag);

	void ProcessAbilityInput(float DeltaTime, bool bGamePaused);
	void ClearAbilityInput();

	// Uses a gameplay effect to add the specified dynamic granted tag.
	void AddDynamicTagGameplayEffect(const FGameplayTag& Tag);

	// Removes all active instances of the gameplay effect that was used to add the specified dynamic granted tag.
	void RemoveDynamicTagGameplayEffect(const FGameplayTag& Tag);

	/** Gets the ability target data associated with the given ability handle and activation info */
	void GetAbilityTargetData(const FGameplayAbilitySpecHandle AbilityHandle, FGameplayAbilityActivationInfo ActivationInfo, FGameplayAbilityTargetDataHandle& OutTargetDataHandle);

	/** Sets the current tag relationship mapping, if null it will clear it out */
	void SetTagRelationshipMapping(UMWAbilityTagRelationshipMapping* NewMapping);
	
	/** Looks at ability tags and gathers additional required and blocking tags */
	void GetAdditionalActivationTagRequirements(const FGameplayTagContainer& AbilityTags, FGameplayTagContainer& OutActivationRequired, FGameplayTagContainer& OutActivationBlocked) const;

protected:
	void TryActivateAbilitiesOnSpawn();

	virtual void AbilitySpecInputPressed(FGameplayAbilitySpec& Spec) override;
	virtual void AbilitySpecInputReleased(FGameplayAbilitySpec& Spec) override;

	virtual void HandleAbilityInputReleasedWithPayload(FGameplayAbilitySpec& Spec, const FMWAbilityInputActionPayload& Payload);

	virtual void NotifyAbilityActivated(const FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability) override;
	virtual void NotifyAbilityFailed(const FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability, const FGameplayTagContainer& FailureReason) override;
	virtual void NotifyAbilityEnded(FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability, bool bWasCancelled) override;
	virtual void ApplyAbilityBlockAndCancelTags(const FGameplayTagContainer& AbilityTags, UGameplayAbility* RequestingAbility, bool bEnableBlockTags, const FGameplayTagContainer& BlockTags, bool bExecuteCancelTags, const FGameplayTagContainer& CancelTags) override;
	virtual void HandleChangeAbilityCanBeCanceled(const FGameplayTagContainer& AbilityTags, UGameplayAbility* RequestingAbility, bool bCanBeCanceled) override;

	void HandleAbilityFailed(const UGameplayAbility* Ability, const FGameplayTagContainer& FailureReason);

protected:
	// Handles to abilities that had their input pressed this frame.
	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;

	// Handles to abilities that had their input released this frame.
	TArray<FMWAbilityReleasedInput> InputReleasedSpecHandlesWithPayload;

	// Handles to abilities that have their input held.
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;

public:
	UFUNCTION(BlueprintCallable, Category = "Gameplay Abilities")
	void FindInstancedAbilityByTag(TArray<UMWGameplayAbility*>& OutAbilities, const FGameplayTagContainer& Tags);

	UFUNCTION(BlueprintCallable, Category = "Gameplay Abilities")
	void FindInstancedAbilityByClass(TArray<UMWGameplayAbility*>& OutAbilities, TSubclassOf<UMWGameplayAbility> InClass);

	//****Interface for Actor****//
public:
	UFUNCTION(BlueprintCallable, Category = "Gameplay Abilities")
	static bool ClearAllAbilitiesByActor(AActor* InActor);

	//****Interface for Actor****//
};
