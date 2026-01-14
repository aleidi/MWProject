// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "EnhancedInputComponent.h"
#include "MWInputConfig.h"
#include "MWInputComponent.generated.h"

class UEnhancedInputLocalPlayerSubsystem;
class UInputAction;
class UObject;

/**
 * UMWInputComponent
 *
 *	Component used to manage input mappings and bindings using an input config data asset.
 */
UCLASS(Config = Input)
class UMWInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	UMWInputComponent(const FObjectInitializer& ObjectInitializer);

	void AddInputMappings(const UMWInputConfig* InputConfig, UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const;
	void RemoveInputMappings(const UMWInputConfig* InputConfig, UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const;

	// Bind a single action by tags (IMC + ActionTag).
	template<class UserClass, typename FuncType>
	void BindActionByTag(const UMWInputConfig* InputConfig, const FGameplayTag& IMCTag, const FGameplayTag& InputActionTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func);

	// Bind all actions inside one IMC, using each action's PressEventType/ReleaseEventType.
	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
	void BindActionsByIMC(const UMWInputConfig* InputConfig, const FGameplayTag& IMCTag, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles);

	void RemoveBinds(TArray<uint32>& BindHandles);
};

template<class UserClass, typename FuncType>
void UMWInputComponent::BindActionByTag(const UMWInputConfig* InputConfig, const FGameplayTag& IMCTag, const FGameplayTag& InputActionTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func)
{
	check(InputConfig);

	if (const UInputAction* IA = InputConfig->FindInputActionForTag(IMCTag, InputActionTag))
	{
		BindAction(IA, TriggerEvent, Object, Func);
	}
}

template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
void UMWInputComponent::BindActionsByIMC(const UMWInputConfig* InputConfig, const FGameplayTag& IMCTag, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles)
{
	check(InputConfig);

	TArray<FMWInputAction> inputActions;
	if (!InputConfig->GetInputActionsForTag(IMCTag, inputActions))
	{
		return;
	}

	for (const FMWInputAction& action : inputActions)
	{
		if (action.InputAction && action.InputTag.IsValid())
		{
			if (PressedFunc)
			{
				BindHandles.Add(BindAction(action.InputAction, action.PressEventType, Object, PressedFunc, action.InputTag).GetHandle());
			}

			if (ReleasedFunc)
			{
				BindHandles.Add(BindAction(action.InputAction, action.ReleaseEventType, Object, ReleasedFunc, action.InputTag).GetHandle());
			}
		}
	}
}