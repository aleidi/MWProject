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
	void BindNativeAction(const UMWInputConfig* InputConfig, const FGameplayTag& IMCTag, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func);

	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
	void BindAbilityActions(const UMWInputConfig* InputConfig, const FGameplayTag& IMCTag, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& OutBindHandles);

	void RemoveBinds(TArray<uint32>& BindHandles);
};


template<class UserClass, typename FuncType>
void UMWInputComponent::BindNativeAction(const UMWInputConfig* InputConfig, const FGameplayTag& IMCTag, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func)
{
	check(InputConfig);
	if (const UInputAction* IA = InputConfig->FindNativeInputActionForTag(IMCTag, InputTag))
	{
		BindAction(IA, TriggerEvent, Object, Func); 
	}
}

template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
void UMWInputComponent::BindAbilityActions(const UMWInputConfig* InputConfig, const FGameplayTag& IMCTag, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& OutBindHandles)
{
	check(InputConfig);

	TArray<FMWInputAction> abilityInputActions;

	bool res = InputConfig->GetAbilityInputActionsForTag(IMCTag, abilityInputActions);

	if (!res)
	{
		return;
	}

	for (const FMWInputAction& action : abilityInputActions)
	{
		if (action.InputAction && action.InputTag.IsValid())
		{
			if (PressedFunc)
			{
				OutBindHandles.Add(BindAction(action.InputAction, ETriggerEvent::Started, Object, PressedFunc, action.InputTag).GetHandle());
			}

			if (ReleasedFunc)
			{
				OutBindHandles.Add(BindAction(action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, action.InputTag).GetHandle());
			}
		}
	}
}
