// Copyright Epic Games, Inc. All Rights Reserved.

#include "Input/MWInputConfig.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "MWLogChannels.h"

const UInputAction* UMWInputConfig::FindNativeInputActionForTag(const FGameplayTag& InputActionTag, bool bLogNotFound) const
{
	for (const FMWInputAction& Action : NativeInputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputActionTag))
		{
			return Action.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogMW, Error, TEXT("Can't find NativeInputAction for InputTag [%s] on InputConfig [%s]."), *InputActionTag.ToString(), *GetNameSafe(this));
	}

	return nullptr;
}

const UInputAction* UMWInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputActionTag, bool bLogNotFound) const
{
	for (const FMWInputAction& Action : AbilityInputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputActionTag))
		{
			return Action.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogMW, Error, TEXT("Can't find AbilityInputAction for InputTag [%s] on InputConfig [%s]."), *InputActionTag.ToString(), *GetNameSafe(this));
	}

	return nullptr;
}