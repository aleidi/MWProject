// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "MW.h"
#include "GameplayTagContainer.h"
#include "GameplayAbilitySpec.h"
#include "GameplayEffectTypes.h"
#include "Engine/DataAsset.h"

#include "MWAbilitySet.generated.h"


class UMWAbilitySystemComponent;
class UMWGameplayAbility;
class UGameplayEffect;


/**
 * FMWAbilitySet_GameplayAbility
 *
 *	AbilitySetからGameplayAbilityを付与するためのデータです。
 */
USTRUCT(BlueprintType)
struct FMWAbilitySet_GameplayAbility
{
	GENERATED_BODY()

public:

	// 付与するGameplayAbility。
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMWGameplayAbility> Ability = nullptr;

	// 付与するAbilityのLevel。
	UPROPERTY(EditDefaultsOnly)
	int32 AbilityLevel = 1;

	// Abilityの入力処理に使用するTag。
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag;
};


/**
 * FMWAbilitySet_GameplayEffect
 *
 *	AbilitySetからGameplayEffectを付与するためのデータです。
 */
USTRUCT(BlueprintType)
struct FMWAbilitySet_GameplayEffect
{
	GENERATED_BODY()

public:

	// 付与するGameplayEffect。
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> GameplayEffect = nullptr;

	// 付与するGameplayEffectのLevel。
	UPROPERTY(EditDefaultsOnly)
	float EffectLevel = 1.0f;
};

/**
 * FMWAbilitySet_AttributeSet
 *
 *	AbilitySetからAttributeSetを付与するためのデータです。
 */
USTRUCT(BlueprintType)
struct FMWAbilitySet_AttributeSet
{
	GENERATED_BODY()

public:
	// 付与するAttributeSet。
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAttributeSet> AttributeSet;

};

/**
 * FMWAbilitySet_GrantedHandles
 *
 *	AbilitySetから付与した要素のHandleを保持するデータです。
 */
USTRUCT()
struct FMWAbilitySet_GrantedHandles
{
	GENERATED_USTRUCT_BODY()

public:
	void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle);
	void AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle);
	void AddAttributeSet(UAttributeSet* Set);

	void RemoveFromAbilitySystem(UMWAbilitySystemComponent* MWASC);

protected:
	// 付与したAbilityのHandle。
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;

	// 付与したGameplayEffectのHandle。
	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> GameplayEffectHandles;

	// 付与したAttributeSetへのPointer。
	UPROPERTY()
	TArray<TObjectPtr<UAttributeSet>> GrantedAttributeSets;
};


/**
 * UMWAbilitySet
 *
 *	GameplayAbilityとGameplayEffectの付与に使用する変更不可のDataAssetです。
 */
UCLASS(BlueprintType, Const)
class UMWAbilitySet : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UMWAbilitySet(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// 指定したAbilitySystemComponentへAbilitySetを付与します。
	// 戻り値のHandleは、付与した要素を後から解除する際に使用できます。
	void GiveToAbilitySystem(UMWAbilitySystemComponent* MWASC, FMWAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject = nullptr) const;

protected:

	// AbilitySet付与時に付与するGameplayAbility。
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities", meta=(TitleProperty=Ability))
	TArray<FMWAbilitySet_GameplayAbility> GrantedGameplayAbilities;

	// AbilitySet付与時に付与するGameplayEffect。
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Effects", meta=(TitleProperty=GameplayEffect))
	TArray<FMWAbilitySet_GameplayEffect> GrantedGameplayEffects;

	// AbilitySet付与時に付与するAttributeSet。
	UPROPERTY(EditDefaultsOnly, Category = "Attribute Sets", meta=(TitleProperty=AttributeSet))
	TArray<FMWAbilitySet_AttributeSet> GrantedAttributes;
};
