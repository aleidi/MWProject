// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameplayAbility/Attribute/MWBattleAttributeSet.h"
#include "GameplayAbility/Attribute/MWAttributeSet.h"
#include "GameplayAbility/MWAbilitySystemComponent.h"
#include "GameplayEffectExtension.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MWBattleAttributeSet)

UMWBattleAttributeSet::UMWBattleAttributeSet()
{
	Strength = 0.f;
	Endurance = 0.f;
	Dexterity = 0.f;
	Agility = 0.f;
	Luck = 0.f;
	Health = 0.f;
	MaxHealth = 0.f;
	HitRate = 0.f;
	Damage = 0.f;
}

void UMWBattleAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

}

bool UMWBattleAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData &Data)
{
	if (!Super::PreGameplayEffectExecute(Data))
	{
		return false;
	}

	return true;
}

void UMWBattleAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		// derive the damage and reset damage property 
		const float damage = GetDamage();
		SetDamage(0.f);

		// apply the damage to health
		const float curr_hp = GetHealth();
		SetHealth(FMath::Clamp(curr_hp - damage, 0.f, GetMaxHealth()));
	}
}

void UMWBattleAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	ClampAttribute(Attribute, NewValue);
}

void UMWBattleAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	ClampAttribute(Attribute, NewValue);
}

void UMWBattleAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
}

void UMWBattleAttributeSet::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetHealthAttribute())
	{
		// Do not allow health to go negative or above max health.
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	else if (Attribute == GetMaxHealthAttribute())
	{
		// Do not allow max health to drop below 1.
		NewValue = FMath::Max(NewValue, 1.0f);
	}
}

void UMWBattleAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMWBattleAttributeSet, Strength, OldValue);
}

void UMWBattleAttributeSet::OnRep_Endurance(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMWBattleAttributeSet, Endurance, OldValue);
}

void UMWBattleAttributeSet::OnRep_Dexterity(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMWBattleAttributeSet, Dexterity, OldValue);
}

void UMWBattleAttributeSet::OnRep_Agility(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMWBattleAttributeSet, Agility, OldValue);
}

void UMWBattleAttributeSet::OnRep_Luck(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMWBattleAttributeSet, Luck, OldValue);
}

void UMWBattleAttributeSet::OnRep_HP(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMWBattleAttributeSet, Health, OldValue);
}

void UMWBattleAttributeSet::OnRep_MaxHP(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMWBattleAttributeSet, MaxHealth, OldValue);
}