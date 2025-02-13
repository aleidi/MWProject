// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameplayAbility/Attribute/MWBattleSet.h"
#include "GameplayAbility/Attribute/MWAttributeSet.h"
#include "GameplayAbility/MWAbilitySystemComponent.h"
#include "../Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/GameplayEffectExtension.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MWBattleSet)

UMWBattleSet::UMWBattleSet()
{
	Strength = 0.f;
	Endurance = 0.f;
	Dexterity = 0.f;
	Agility = 0.f;
	Luck = 0.f;
	HP = 0.f;
	MaxHP = 0.f;
	HitRate = 0.f;
	Damage = 0.f;
}

void UMWBattleSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

}

bool UMWBattleSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData &Data)
{
	if (!Super::PreGameplayEffectExecute(Data))
	{
		return false;
	}

	return true;
}

void UMWBattleSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		// derive the damage and reset damage property 
		const float damage = GetDamage();
		SetDamage(0.f);

		// apply the damage to health
		const float curr_hp = GetHP();
		SetHP(FMath::Clamp(curr_hp - damage, 0.f, GetMaxHP()));
	}
}

void UMWBattleSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	ClampAttribute(Attribute, NewValue);
}

void UMWBattleSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	ClampAttribute(Attribute, NewValue);
}

void UMWBattleSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
}

void UMWBattleSet::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetHPAttribute())
	{
		// Do not allow health to go negative or above max health.
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHP());
	}
	else if (Attribute == GetMaxHPAttribute())
	{
		// Do not allow max health to drop below 1.
		NewValue = FMath::Max(NewValue, 1.0f);
	}
}

void UMWBattleSet::OnRep_Strength(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMWBattleSet, Strength, OldValue);
}

void UMWBattleSet::OnRep_Endurance(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMWBattleSet, Endurance, OldValue);
}

void UMWBattleSet::OnRep_Dexterity(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMWBattleSet, Dexterity, OldValue);
}

void UMWBattleSet::OnRep_Agility(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMWBattleSet, Agility, OldValue);
}

void UMWBattleSet::OnRep_Luck(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMWBattleSet, Luck, OldValue);
}

void UMWBattleSet::OnRep_HP(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMWBattleSet, HP, OldValue);
}

void UMWBattleSet::OnRep_MaxHP(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMWBattleSet, MaxHP, OldValue);
}