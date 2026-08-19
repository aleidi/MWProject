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
 * 複数のAbilitySystemComponent（ASC）へGlobal適用するAbilityを管理します。
 * ASCごとの付与状況を追跡し、追加／削除処理を提供します。
 */
USTRUCT()
struct FGlobalAppliedAbilityList
{
	GENERATED_BODY()

	/** ASCと付与済みAbilitySpecHandleのMap。GlobalAbilityの追跡と管理に使用します。 */
	UPROPERTY()
	TMap<TObjectPtr<UMWAbilitySystemComponent>, FGameplayAbilitySpecHandle> Handles;

	/** 指定したAbilityをASCへ付与し、後の削除に使用するHandleを保持します。 */
	void AddToASC(TSubclassOf<UGameplayAbility> Ability, UMWAbilitySystemComponent* ASC);
	
	/** 指定したASCから付与済みAbilityを削除します。 */
	void RemoveFromASC(UMWAbilitySystemComponent* ASC);
	
	/** このAbilityが付与されている全ASCから削除します。 */
	void RemoveFromAll();
};

/**
 * 複数のAbilitySystemComponent（ASC）へGlobal適用するGameplayEffectを管理します。
 * ASCごとの適用状況を追跡し、追加／削除処理を提供します。
 */
USTRUCT()
struct FGlobalAppliedEffectList
{
	GENERATED_BODY()

	/** ASCと適用済みActiveGameplayEffectHandleのMap。GlobalEffectの追跡と管理に使用します。 */
	UPROPERTY()
	TMap<TObjectPtr<UMWAbilitySystemComponent>, FActiveGameplayEffectHandle> Handles;

	/** 指定したGameplayEffectをASCへ適用し、後の削除に使用するHandleを保持します。 */
	void AddToASC(TSubclassOf<UGameplayEffect> Effect, UMWAbilitySystemComponent* ASC);
	
	/** 指定したASCから適用済みEffectを削除します。 */
	void RemoveFromASC(UMWAbilitySystemComponent* ASC);
	
	/** このEffectが適用されている全ASCから削除します。 */
	void RemoveFromAll();
};

/**
 * GlobalAbilityとGameplayEffectを管理するWorldSubsystemです。
 * 
 * World内の登録済みAbilitySystemComponentへAbilityとEffectを一括適用します。
 * 新規登録されたASCには有効なGlobalAbility／Effectを自動適用し、登録解除時に自動削除します。
 * 
 * 主な用途：
 * - 全プレイヤーへ影響するゲーム全体のBuff／Debuff
 * - 全対象へ適用する環境Effect
 * - 全Clientに必要なServer Authority Ability
 */
UCLASS()
class UMWGlobalAbilitySystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UMWGlobalAbilitySystem();

	/**
	 * 現在および今後登録される全ASCへ指定Abilityを付与します。
	 * Server／Authority専用であり、Clientでは動作しません。
	 * 
	 * @param Ability Globalに付与するGameplayAbilityクラス。
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="MW")
	void ApplyAbilityToAll(TSubclassOf<UGameplayAbility> Ability);

	/**
	 * 現在および今後登録される全ASCへ指定GameplayEffectを適用します。
	 * Server／Authority専用であり、Clientでは動作しません。
	 * 
	 * @param Effect Globalに適用するGameplayEffectクラス。
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="MW")
	void ApplyEffectToAll(TSubclassOf<UGameplayEffect> Effect);

	/**
	 * 指定Abilityが付与されている全ASCから削除します。
	 * Server／Authority専用であり、Clientでは動作しません。
	 * 
	 * @param Ability Globalから削除するGameplayAbilityクラス。
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "MW")
	void RemoveAbilityFromAll(TSubclassOf<UGameplayAbility> Ability);

	/**
	 * 指定GameplayEffectが適用されている全ASCから削除します。
	 * Server／Authority専用であり、Clientでは動作しません。
	 * 
	 * @param Effect Globalから削除するGameplayEffectクラス。
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "MW")
	void RemoveEffectFromAll(TSubclassOf<UGameplayEffect> Effect);

	/**
	 * ASCをGlobalSystemへ登録し、現在有効なGlobalEffect／Abilityを自動適用します。
	 * ASCの生成後、GlobalAbility／Effectを受け取れる状態で呼び出します。
	 * 
	 * @param ASC 登録するAbilitySystemComponent。
	 */
	void RegisterASC(UMWAbilitySystemComponent* ASC);

	/**
	 * ASCをGlobalSystemから登録解除し、有効なGlobalEffect／Abilityを削除します。
	 * ASCの破棄時またはGlobalAbility／Effectが不要になった際に呼び出します。
	 * 
	 * @param ASC 登録解除するAbilitySystemComponent。
	 */
	void UnregisterASC(UMWAbilitySystemComponent* ASC);

private:
	/** 
	 * Abilityクラスと適用一覧のMapです。
	 * 各要素で対象GlobalAbilityが付与された全ASCを追跡します。
	 */
	UPROPERTY()
	TMap<TSubclassOf<UGameplayAbility>, FGlobalAppliedAbilityList> AppliedAbilities;

	/** 
	 * GameplayEffectクラスと適用一覧のMapです。
	 * 各要素で対象GlobalEffectが適用された全ASCを追跡します。
	 */
	UPROPERTY()
	TMap<TSubclassOf<UGameplayEffect>, FGlobalAppliedEffectList> AppliedEffects;

	/** 
	 * GlobalSystemへ現在登録されている全ASCの一覧です。
	 * 新しいGlobalAbility／Effectの追加時に使用します。
	 */
	UPROPERTY()
	TArray<TObjectPtr<UMWAbilitySystemComponent>> RegisteredASCs;
};
