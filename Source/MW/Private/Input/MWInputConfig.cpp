// Copyright Epic Games, Inc. All Rights Reserved.

#include "Input/MWInputConfig.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "MWLogChannels.h"

const UInputAction* UMWInputConfig::FindNativeInputActionForTag(const FGameplayTag& IMCTag, const FGameplayTag& InputActionTag) const
{
	const FMWInputActionContainer* container = InputActionsContainers.Find(IMCTag);

	if (!container)
	{
		UE_LOG(LogMWInput, Error, TEXT("Can't find Input Action Container for the mapping context [%s]."), *IMCTag.GetTagName().ToString());

		return nullptr;
	}

	for (const FMWInputAction& action : container->NativeInputActions)
	{
		if (action.InputAction && (action.InputTag == InputActionTag))
		{
			return action.InputAction;
		}
	}

	UE_LOG(LogMWInput, Error, TEXT("Can't find NativeInputAction for InputTag [%s] on InputConfig [%s]."), *InputActionTag.ToString(), *GetNameSafe(this));

	return nullptr;
}

const UInputAction* UMWInputConfig::FindAbilityInputActionForTag(const FGameplayTag& IMCTag, const FGameplayTag& InputActionTag) const
{
	const FMWInputActionContainer* container = InputActionsContainers.Find(IMCTag);

	if (!container)
	{
		UE_LOG(LogMWInput, Error, TEXT("Can't find Input Action Container for the mapping context [%s]."), *IMCTag.GetTagName().ToString());

		return nullptr;
	}

	for (const FMWInputAction& Action : container->AbilityInputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputActionTag))
		{
			return Action.InputAction;
		}
	}

	UE_LOG(LogMWInput, Error, TEXT("Can't find AbilityInputAction for InputTag [%s] on InputConfig [%s]."), *InputActionTag.ToString(), *GetNameSafe(this));

	return nullptr;
}

bool UMWInputConfig::GetAbilityInputActionsForTag(const FGameplayTag& IMCTag, TArray<FMWInputAction>& OutActions) const
{
	const FMWInputActionContainer* container = InputActionsContainers.Find(IMCTag);

	if (!container)
	{
		return false;
	}

	OutActions = container->AbilityInputActions;

	return true;
}
