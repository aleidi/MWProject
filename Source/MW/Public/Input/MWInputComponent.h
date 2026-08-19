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
 *	入力設定データアセットを使用して入力マッピングとバインドを管理するコンポーネント。
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

				// ゲームパッド入力用の特殊処理。
				// Chordアクションを使用すると、特殊なケースでゲームパッド入力のCompletedイベントが発生しない場合があります。
				// 入力がキャンセルされた場合にもReleasedFuncが呼び出されるよう、Canceledイベントもバインドします。
				if (action.bBindCancelToReleasedEvent)
				{
					OutBindHandles.Add(BindAction(action.InputAction, ETriggerEvent::Canceled, Object, ReleasedFunc, action.InputTag).GetHandle());
				}
			}
		}
	}
}
