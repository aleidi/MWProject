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

MW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_AbilityInputBlocked);

/**
 * UMWAbilitySystemComponent
 *
 *	本プロジェクトで使用するAbilitySystemComponentの基底クラスです。
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class MW_API UMWAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UMWAbilitySystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~UActorComponentインターフェース
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~UActorComponentインターフェース終了

	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;

	typedef TFunctionRef<bool(const UMWGameplayAbility* MWAbility, FGameplayAbilitySpecHandle Handle)> TShouldCancelAbilityFunc;
	void CancelAbilitiesByFunc(TShouldCancelAbilityFunc ShouldCancelFunc, bool bReplicateCancelAbility);

	void CancelInputActivatedAbilities(bool bReplicateCancelAbility);

	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	/* ActorのAbilityInputTagPressedを呼び出すHelper関数。 */
	static bool ActorAbilityInputTagPressed(AActor* InActor, const FGameplayTag& InputTag);
	/* ActorのAbilityInputTagReleasedを呼び出すHelper関数。 */
	static bool ActorAbilityInputTagReleased(AActor* InActor, const FGameplayTag& InputTag);

	void ProcessAbilityInput(float DeltaTime, bool bGamePaused);
	void ClearAbilityInput();

	bool IsActivationGroupBlocked(EMWAbilityActivationGroup Group) const;
	void AddAbilityToActivationGroup(EMWAbilityActivationGroup Group, UMWGameplayAbility* MWAbility);
	void RemoveAbilityFromActivationGroup(EMWAbilityActivationGroup Group, UMWGameplayAbility* MWAbility);
	void CancelActivationGroupAbilities(EMWAbilityActivationGroup Group, UMWGameplayAbility* IgnoreMWAbility, bool bReplicateCancelAbility);

	// GameplayEffectを使用して指定したDynamicGrantedTagを追加します。
	void AddDynamicTagGameplayEffect(const FGameplayTag& Tag);

	// 指定したDynamicGrantedTagの追加に使用したGameplayEffectの有効なInstanceをすべて削除します。
	void RemoveDynamicTagGameplayEffect(const FGameplayTag& Tag);

	/** 指定したAbilityHandleとActivationInfoに対応するAbilityTargetDataを取得します。 */
	void GetAbilityTargetData(const FGameplayAbilitySpecHandle AbilityHandle, FGameplayAbilityActivationInfo ActivationInfo, FGameplayAbilityTargetDataHandle& OutTargetDataHandle);

	/** 現在のTagRelationshipMappingを設定します。nullの場合は解除します。 */
	void SetTagRelationshipMapping(UMWAbilityTagRelationshipMapping* NewMapping);
	
	/** AbilityTagから追加の必須TagとBlockTagを収集します。 */
	void GetAdditionalActivationTagRequirements(const FGameplayTagContainer& AbilityTags, FGameplayTagContainer& OutActivationRequired, FGameplayTagContainer& OutActivationBlocked) const;

	void OnAbilityFailedToActivate(const FGameplayTagContainer& FailedReason) const;

protected:
	void TryActivateAbilitiesOnSpawn();

	virtual void AbilitySpecInputPressed(FGameplayAbilitySpec& Spec) override;
	virtual void AbilitySpecInputReleased(FGameplayAbilitySpec& Spec) override;

	virtual void NotifyAbilityActivated(const FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability) override;
	virtual void NotifyAbilityFailed(const FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability, const FGameplayTagContainer& FailureReason) override;
	virtual void NotifyAbilityEnded(FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability, bool bWasCancelled) override;
	virtual void ApplyAbilityBlockAndCancelTags(const FGameplayTagContainer& AbilityTags, UGameplayAbility* RequestingAbility, bool bEnableBlockTags, const FGameplayTagContainer& BlockTags, bool bExecuteCancelTags, const FGameplayTagContainer& CancelTags) override;
	virtual void HandleChangeAbilityCanBeCanceled(const FGameplayTagContainer& AbilityTags, UGameplayAbility* RequestingAbility, bool bCanBeCanceled) override;

	/** Abilityの有効化失敗をClientへ通知します。 */
	UFUNCTION(Client, Unreliable)
	void ClientNotifyAbilityFailed(const UGameplayAbility* Ability, const FGameplayTagContainer& FailureReason);
	
	void HandleAbilityFailed(const UGameplayAbility* Ability, const FGameplayTagContainer& FailureReason);

protected:
	// このFrameで入力されたAbilityのHandle。
	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;

	// このFrameで入力が解放されたAbilityのHandle。
	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;

	// 入力が継続中のAbilityのHandle。
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;

	// ActivationGroupごとに実行中のAbility数。
	int32 ActivationGroupCounts[(uint8)EMWAbilityActivationGroup::MAX];

public:
	UFUNCTION(BlueprintCallable, Category = "Gameplay Abilities")
	void FindInstancedAbilityByTag(TArray<UMWGameplayAbility*>& OutAbilities, const FGameplayTagContainer& Tags);

	UFUNCTION(BlueprintCallable, Category = "Gameplay Abilities")
	void FindInstancedAbilityByClass(TArray<UMWGameplayAbility*>& OutAbilities, TSubclassOf<UMWGameplayAbility> InClass);

//****Actor向けインターフェース****//
public:
	UFUNCTION(BlueprintCallable, Category = "Gameplay Abilities")
	static bool ClearAllAbilitiesByActor(AActor* InActor);
//****Actor向けインターフェース****//

};
