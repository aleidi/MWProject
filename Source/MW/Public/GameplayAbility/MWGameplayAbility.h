#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "MWGameplayAbility.generated.h"

/**
 * EMWAbilityActivationPolicy
 *
 *	Defines how an ability is meant to activate.
 */
UENUM(BlueprintType)
enum class EMWAbilityActivationPolicy : uint8
{
	// Try to activate the ability when the input is triggered.
	OnInputTriggered,

	// Continually try to activate the ability while the input is active.
	WhileInputActive,

	// Try to activate the ability when an avatar is assigned.
	OnSpawn
};

UCLASS()
class MW_API UMWGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UMWGameplayAbility(const FObjectInitializer& ObjectInitializer);

public:
	FORCEINLINE EMWAbilityActivationPolicy GetActivationPolicy() const { return ActivationPolicy; }

	/* Allow blueprint to receive input event. */
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	
	/* Allow blueprint to receive release event. */
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual void OnEndAbility(const FGameplayAbilitySpecHandle handle, const FGameplayAbilityActorInfo* actorInfo, const FGameplayAbilityActivationInfo activationInfo, bool bReplicateEndAbility, bool bWasCancelled) {}

	/* ActorInfo maybe null. */
	UFUNCTION(BlueprintImplementableEvent, Category = "Input")
	void K2_InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo& ActorInfo, const FGameplayAbilityActivationInfo ActivationInf);

	/* ActorInfo maybe null. */
	UFUNCTION(BlueprintImplementableEvent, Category = "Input")
	void K2_InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo& ActorInfo, const FGameplayAbilityActivationInfo ActivationInf);

protected:
	/* Defines how this ability is meant to activate. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MW|Ability Activation")
	EMWAbilityActivationPolicy ActivationPolicy;
};