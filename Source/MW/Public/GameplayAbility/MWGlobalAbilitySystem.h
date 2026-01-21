// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ActiveGameplayEffectHandle.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameplayAbilitySpecHandle.h"
#include "Templates/SubclassOf.h"

#include "MWGlobalAbilitySystem.generated.h"

class UGameplayAbility;
class UGameplayEffect;
class UMWAbilitySystemComponent;
class UObject;
struct FActiveGameplayEffectHandle;
struct FFrame;
struct FGameplayAbilitySpecHandle;

/**
 * Manages a collection of globally applied abilities across multiple Ability System Components (ASCs).
 * Tracks which abilities have been granted to which ASCs and provides operations to add/remove them.
 */
USTRUCT()
struct FGlobalAppliedAbilityList
{
	GENERATED_BODY()

	/** Map of ASC to the ability spec handle granted to that ASC. Used to track and manage global abilities. */
	UPROPERTY()
	TMap<TObjectPtr<UMWAbilitySystemComponent>, FGameplayAbilitySpecHandle> Handles;

	/** Grants the specified ability to the given ASC and stores the handle for later removal. */
	void AddToASC(TSubclassOf<UGameplayAbility> Ability, UMWAbilitySystemComponent* ASC);
	
	/** Removes the previously granted ability from the specified ASC. */
	void RemoveFromASC(UMWAbilitySystemComponent* ASC);
	
	/** Removes this ability from all ASCs that currently have it granted. */
	void RemoveFromAll();
};

/**
 * Manages a collection of globally applied gameplay effects across multiple Ability System Components (ASCs).
 * Tracks which effects have been applied to which ASCs and provides operations to add/remove them.
 */
USTRUCT()
struct FGlobalAppliedEffectList
{
	GENERATED_BODY()

	/** Map of ASC to the active gameplay effect handle applied to that ASC. Used to track and manage global effects. */
	UPROPERTY()
	TMap<TObjectPtr<UMWAbilitySystemComponent>, FActiveGameplayEffectHandle> Handles;

	/** Applies the specified gameplay effect to the given ASC and stores the handle for later removal. */
	void AddToASC(TSubclassOf<UGameplayEffect> Effect, UMWAbilitySystemComponent* ASC);
	
	/** Removes the previously applied effect from the specified ASC. */
	void RemoveFromASC(UMWAbilitySystemComponent* ASC);
	
	/** Removes this effect from all ASCs that currently have it applied. */
	void RemoveFromAll();
};

/**
 * World subsystem that manages global abilities and gameplay effects.
 * 
 * This system allows you to apply abilities and effects to all registered Ability System Components
 * in the world at once. When new ASCs register, they automatically receive any active global abilities/effects.
 * When ASCs unregister, their global abilities/effects are automatically cleaned up.
 * 
 * Common use cases:
 * - Game-wide buffs/debuffs that affect all players
 * - Environmental effects that should apply to everyone
 * - Server-authoritative abilities that all clients need
 */
UCLASS()
class UMWGlobalAbilitySystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UMWGlobalAbilitySystem();

	/**
	 * Grants the specified ability to all currently registered ASCs and any ASCs that register in the future.
	 * Server/Authority only - will not work on clients.
	 * 
	 * @param Ability - The gameplay ability class to grant globally
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="MW")
	void ApplyAbilityToAll(TSubclassOf<UGameplayAbility> Ability);

	/**
	 * Applies the specified gameplay effect to all currently registered ASCs and any ASCs that register in the future.
	 * Server/Authority only - will not work on clients.
	 * 
	 * @param Effect - The gameplay effect class to apply globally
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="MW")
	void ApplyEffectToAll(TSubclassOf<UGameplayEffect> Effect);

	/**
	 * Removes the specified ability from all ASCs that currently have it granted.
	 * Server/Authority only - will not work on clients.
	 * 
	 * @param Ability - The gameplay ability class to remove globally
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "MW")
	void RemoveAbilityFromAll(TSubclassOf<UGameplayAbility> Ability);

	/**
	 * Removes the specified gameplay effect from all ASCs that currently have it applied.
	 * Server/Authority only - will not work on clients.
	 * 
	 * @param Effect - The gameplay effect class to remove globally
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "MW")
	void RemoveEffectFromAll(TSubclassOf<UGameplayEffect> Effect);

	/**
	 * Registers an ASC with the global system and automatically applies any currently active global effects/abilities.
	 * Should be called when an ASC is created and ready to receive global abilities/effects.
	 * 
	 * @param ASC - The Ability System Component to register
	 */
	void RegisterASC(UMWAbilitySystemComponent* ASC);

	/**
	 * Unregisters an ASC from the global system and removes any active global effects/abilities from it.
	 * Should be called when an ASC is being destroyed or no longer needs global abilities/effects.
	 * 
	 * @param ASC - The Ability System Component to unregister
	 */
	void UnregisterASC(UMWAbilitySystemComponent* ASC);

private:
	/** 
	 * Maps ability classes to their application lists.
	 * Each entry tracks all ASCs that have been granted that particular global ability.
	 */
	UPROPERTY()
	TMap<TSubclassOf<UGameplayAbility>, FGlobalAppliedAbilityList> AppliedAbilities;

	/** 
	 * Maps gameplay effect classes to their application lists.
	 * Each entry tracks all ASCs that have that particular global effect applied.
	 */
	UPROPERTY()
	TMap<TSubclassOf<UGameplayEffect>, FGlobalAppliedEffectList> AppliedEffects;

	/** 
	 * List of all ASCs currently registered with the global system.
	 * Used to apply new global abilities/effects when they are added.
	 */
	UPROPERTY()
	TArray<TObjectPtr<UMWAbilitySystemComponent>> RegisteredASCs;
};
