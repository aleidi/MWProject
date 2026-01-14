// Copyright Epic Games, Inc. All Rights Reserved.

#include "Input/MWInputConfig.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "MWLogChannels.h"

const UInputAction* UMWInputConfig::FindInputActionForTag(const FGameplayTag& IMCTag, const FGameplayTag& InputActionTag) const
{
	const FMWInputActionContainer* container = InputActionsContainers.Find(IMCTag);

	if (!container)
	{
		UE_LOG(LogMWInput, Error, TEXT("Can't find Input Action Container for the mapping context [%s]."), *IMCTag.GetTagName().ToString());
		return nullptr;
	}

	for (const FMWInputAction& action : container->InputActions)
	{
		if (action.InputAction && (action.InputTag == InputActionTag))
		{
			return action.InputAction;
		}
	}

	UE_LOG(LogMWInput, Error, TEXT("Can't find InputAction for InputTag [%s] on InputConfig [%s]."), *InputActionTag.ToString(), *GetNameSafe(this));
	return nullptr;
}

bool UMWInputConfig::GetInputActionsForTag(const FGameplayTag& IMCTag, TArray<FMWInputAction>& OutActions) const
{
	const FMWInputActionContainer* container = InputActionsContainers.Find(IMCTag);
	if (!container)
	{
		return false;
	}

	OutActions = container->InputActions;
	return true;
}

bool UMWInputConfig::GetInputActionForTag(const FGameplayTag& IMCTag, const FGameplayTag& InputActionTag, FMWInputAction& OutAction) const
{
	const FMWInputActionContainer* container = InputActionsContainers.Find(IMCTag);
	if (!container)
	{
		return false;
	}

	for (const FMWInputAction& action : container->InputActions)
	{
		if (action.InputTag == InputActionTag)
		{
			OutAction = action;
			return true;
		}
	}

	return false;
}
