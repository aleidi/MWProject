// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameplayAbility/Attribute/MWAttributeSet.h"
#include "GameplayAbility/MWAbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MWAttributeSet)

class UWorld;

UMWAttributeSet::UMWAttributeSet()
{
}

UWorld* UMWAttributeSet::GetWorld() const
{
	const UObject* Outer = GetOuter();
	check(Outer);

	return Outer->GetWorld();
}

UMWAbilitySystemComponent* UMWAttributeSet::GetMWAbilitySystemComponent() const
{
	return Cast<UMWAbilitySystemComponent>(GetOwningAbilitySystemComponent());
}

