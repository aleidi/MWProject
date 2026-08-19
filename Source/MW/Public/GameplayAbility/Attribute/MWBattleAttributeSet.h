// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "AbilitySystemComponent.h"
#include "MWAttributeSet.h"
#include "NativeGameplayTags.h"

#include "MWBattleAttributeSet.generated.h"

class UObject;
struct FFrame;

//UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Damage);
//UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_DamageImmunity);
//UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_DamageSelfDestruct);
//UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_FellOutOfWorld);
//UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_MW_Damage_Message);

struct FGameplayEffectModCallbackData;


/**
 * @class UMWBattleAttributeSet
 *
 * @brief 戦闘中のキャラクターが使用するAttributeです。
 *	      戦闘中のキャラクターに使用される属性.
 */
UCLASS(BlueprintType)
class UMWBattleAttributeSet : public UMWAttributeSet
{
	GENERATED_BODY()

public:

	UMWBattleAttributeSet();

	//// ダメージ／DexterityによりStrengthが変化した際のDelegate。クライアントでは一部情報が欠ける場合があります。
	//mutable FMWAttributeEvent OnStrengthChanged;

	//// Strength最大値が変化した際のDelegate
	//mutable FMWAttributeEvent OnEnduranceChanged;

	//// Strength Attributeが0になった際に通知するDelegate
	//mutable FMWAttributeEvent OnOutOfStrength;

protected:
	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const;

public:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Strength, Category = "MW", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UMWBattleAttributeSet, Strength);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Endurance, Category = "MW", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Endurance;
	ATTRIBUTE_ACCESSORS(UMWBattleAttributeSet, Endurance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Dexterity, Category = "MW", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Dexterity;
	ATTRIBUTE_ACCESSORS(UMWBattleAttributeSet, Dexterity);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Agility, Category = "MW", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Agility;
	ATTRIBUTE_ACCESSORS(UMWBattleAttributeSet, Agility);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Luck, Category = "MW", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Luck;
	ATTRIBUTE_ACCESSORS(UMWBattleAttributeSet, Luck);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HP, Category = "MW", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UMWBattleAttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHP, Category = "MW", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UMWBattleAttributeSet, MaxHealth);

	// -------------------------------------------------------------------
	//	一時Attribute
	// -------------------------------------------------------------------
	UPROPERTY(BlueprintReadOnly, Category="MW", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData HitRate;
	ATTRIBUTE_ACCESSORS(UMWBattleAttributeSet, HitRate);

	UPROPERTY(BlueprintReadOnly, Category="MW", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UMWBattleAttributeSet, Damage);

	UPROPERTY(BlueprintReadOnly, Category="MW", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData ComboTotalDamage;
	ATTRIBUTE_ACCESSORS(UMWBattleAttributeSet, ComboTotalDamage);

protected:
	// Replication時にAbilitySystem内部表現を正しく同期するためのOnRep関数です。
	UFUNCTION()
	virtual void OnRep_Strength(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_Endurance(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_Dexterity(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_Agility(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_Luck(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_HP(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_MaxHP(const FGameplayAttributeData& OldValue);
};
