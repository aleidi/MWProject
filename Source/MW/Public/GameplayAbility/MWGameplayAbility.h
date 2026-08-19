#pragma once

#include "MW.h"
#include "Abilities/GameplayAbility.h"
#include "MWGameplayAbility.generated.h"

class UMWAbilitySystemComponent;

/**
 * EMWAbilityActivationPolicy
 *
 *	Abilityの有効化方針を定義します。
 */
UENUM(BlueprintType)
enum class EMWAbilityActivationPolicy : uint8
{
	// 入力発生時にAbilityの有効化を試行します。
	OnInputTriggered,

	// 入力中は継続してAbilityの有効化を試行します。
	WhileInputActive,

	// Avatar割当時にAbilityの有効化を試行します。
	OnSpawn
};

/**
 * EMWAbilityActivationGroup
 *
 *	他のAbilityとの関係に基づく有効化方式を定義します。
 */
UENUM(BlueprintType)
enum class EMWAbilityActivationGroup : uint8
{
	// 他のAbilityから独立して実行します。
	Independent,

	// 他の排他的AbilityによりCancelされ、置き換えられます。
	Exclusive_Replaceable,

	// 他の排他的Abilityの有効化をすべてBlockします。
	Exclusive_Blocking,

	MAX	UMETA(Hidden)
};

UCLASS()
class MW_API UMWGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

	friend class UMWAbilitySystemComponent;

public:
	UMWGameplayAbility(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "MW|Ability")
	UMWAbilitySystemComponent* GetMWAbilitySystemComponentFromActorInfo() const;

	void TryActivateAbilityOnSpawn(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) const;

	// 指定したActivationGroupへ遷移可能な場合はtrueを返します。
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "MW|Ability", Meta = (ExpandBoolAsExecs = "ReturnValue"))
	bool CanChangeActivationGroup(EMWAbilityActivationGroup NewGroup) const;

	// ActivationGroupの変更を試行し、成功した場合はtrueを返します。
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "MW|Ability", Meta = (ExpandBoolAsExecs = "ReturnValue"))
	bool ChangeActivationGroup(EMWAbilityActivationGroup NewGroup);

	EMWAbilityActivationPolicy GetActivationPolicy() const { return ActivationPolicy; }

	EMWAbilityActivationGroup GetActivationGroup() const { return ActivationGroup; }

	/* Blueprintで入力イベントを受信できるようにします。 */
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	
	/* Blueprintで入力解放イベントを受信できるようにします。 */
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;


protected:
	//~UGameplayAbilityインターフェース
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void SetCanBeCanceled(bool bCanBeCanceled) override;
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	// virtual bool CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	// virtual void ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;
	// virtual FGameplayEffectContextHandle MakeEffectContext(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo) const override;
	// virtual void ApplyAbilityTagsToGameplayEffectSpec(FGameplayEffectSpec& Spec, FGameplayAbilitySpec* AbilitySpec) const override;
	// virtual bool DoesAbilitySatisfyTagRequirements(const UAbilitySystemComponent& AbilitySystemComponent, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	//~UGameplayAbilityインターフェース終了

	/* AbilitySystemComponentへPawn Avatarが設定された際に呼び出されます。 */
	virtual void OnPawnAvatarSet();

	/** このAbilityがAbilitySystemComponentへ付与された際に呼び出されます。 */
	UFUNCTION(BlueprintImplementableEvent, Category = Ability, DisplayName = "OnAbilityAdded")
	void K2_OnAbilityAdded();

	/** このAbilityがAbilitySystemComponentから削除された際に呼び出されます。 */
	UFUNCTION(BlueprintImplementableEvent, Category = Ability, DisplayName = "OnAbilityRemoved")
	void K2_OnAbilityRemoved();

	/** AbilitySystemがPawn Avatarで初期化された際に呼び出されます。 */
	UFUNCTION(BlueprintImplementableEvent, Category = Ability, DisplayName = "OnPawnAvatarSet")
	void K2_OnPawnAvatarSet();

	/* ActorInfoはnullの場合があります。 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Input")
	void K2_InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo& ActorInfo, const FGameplayAbilityActivationInfo ActivationInf);

	/* ActorInfoはnullの場合があります。 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Input")
	void K2_InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo& ActorInfo, const FGameplayAbilityActivationInfo ActivationInf);


protected:
	/* このAbilityの有効化方針を定義します。 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MW|Ability Activation")
	EMWAbilityActivationPolicy ActivationPolicy;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MW|Ability Activation")
	EMWAbilityActivationGroup ActivationGroup;
};