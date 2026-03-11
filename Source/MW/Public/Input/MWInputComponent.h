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

	template<class UserClass, typename FuncType>
	void BindNativeAction(const UMWInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func, uint32& OutBindHandle);

	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
	void BindAbilityActions(const UMWInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& OutBindHandles);

	void RemoveBinds(TArray<uint32>& BindHandles);
};


template<class UserClass, typename FuncType>
void UMWInputComponent::BindNativeAction(const UMWInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func, uint32& OutBindHandle)
{
	check(InputConfig);
	if (const UInputAction* IA = InputConfig->FindNativeInputActionForTag(InputTag))
	{
		OutBindHandle = BindAction(IA, TriggerEvent, Object, Func).GetHandle();
	}
}

template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
void UMWInputComponent::BindAbilityActions(const UMWInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& OutBindHandles)
{
	check(InputConfig);

	TArray<FMWInputAction> abilityInputActions;

	if (!InputConfig->GetAbilityInputActions(abilityInputActions))
	{
		return;
	}

	for (const FMWInputAction& action : abilityInputActions)
	{
		if (action.InputAction && action.InputTag.IsValid())
		{
			if (PressedFunc)
			{
				OutBindHandles.Add(BindAction(action.InputAction, action.PressedTriggerEvent, Object, PressedFunc, action.InputTag).GetHandle());
			}

			if (ReleasedFunc)
			{
				OutBindHandles.Add(BindAction(action.InputAction, action.ReleasedTriggerEvent, Object, ReleasedFunc, action.InputTag).GetHandle());

				// Special process for gamepad input.
				// The completed event of gamepad input may not be triggered in some special case when the chord action is used. 
				// So we also bind the canceled event to make sure the released func can be called when the input is canceled.
				if (action.bBindCancelToReleasedEvent)
				{
					OutBindHandles.Add(BindAction(action.InputAction, ETriggerEvent::Canceled, Object, ReleasedFunc, action.InputTag).GetHandle());
				}
			}
		}
	}
}
