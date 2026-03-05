// Copyright Epic Games, Inc. All Rights Reserved.

#include "Input/MWInputConfig.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "MWLogChannels.h"

const UInputAction* UMWInputConfig::FindNativeInputActionForTag(const FGameplayTag& InputActionTag) const
{
	for (const FMWInputAction& action : InputActionsContainer.NativeInputActions)
	{
		if (action.InputAction && (action.InputTag == InputActionTag))
		{
			return action.InputAction;
		}
	}

	UE_LOG(LogMWInput, Error, TEXT("Can't find NativeInputAction for InputTag [%s] on InputConfig [%s]."), *InputActionTag.ToString(), *GetNameSafe(this));

	return nullptr;
}

const UInputAction* UMWInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputActionTag) const
{
	for (const FMWInputAction& Action : InputActionsContainer.AbilityInputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputActionTag))
		{
			return Action.InputAction;
		}
	}

	UE_LOG(LogMWInput, Error, TEXT("Can't find AbilityInputAction for InputTag [%s] on InputConfig [%s]."), *InputActionTag.ToString(), *GetNameSafe(this));

	return nullptr;
}

bool UMWInputConfig::GetAbilityInputActions(TArray<FMWInputAction>& OutActions) const
{
	if (InputActionsContainer.AbilityInputActions.IsEmpty())
	{
		return false;
	}

	OutActions = InputActionsContainer.AbilityInputActions;

	return true;
}
