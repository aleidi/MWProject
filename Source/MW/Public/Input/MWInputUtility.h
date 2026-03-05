#pragma once

// Include
#include "CoreMinimal.h"
#include "Define/MWDefineCommon.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputDeveloperSettings.h"
#include "EnhancedInputSubsystemInterface.h"
#include "MWInputHandler.h"
#include "MWLogChannels.h"
#include "System/MWWorldSubsystem.h"
#include "MWInputUtility.generated.h"

// Forward Declare
class UMWInputConfig;

// Define

/*
 * @class UMWInputUtility
 * 
 * @brief Input Utility implements some convenient static functions
 *        to help process input related logic.
 *
 * @note
 */
UCLASS()
class UMWInputUtility : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Input")
	static void EnableMappingContext(APlayerController* PC, const UMWInputConfig* InputConfig, const FModifyContextOptions& Options = FModifyContextOptions());

	UFUNCTION(BlueprintCallable, Category="Input")
	static void DisableMappingContext(APlayerController* PC, const UMWInputConfig* InputConfig, const FModifyContextOptions& Options = FModifyContextOptions());

	template<DerivedFromUObject UserClass, typename FuncType, typename... VarTypes>
	static void BindInputAction(const UInputAction* Action, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func, VarTypes... Params)
	{
		if (!Object)
		{
			return;
		}

		if (!GetDefault<UEnhancedInputDeveloperSettings>()->bEnableWorldSubsystem)
		{
			UE_LOG(LogMWInput, Warning, TEXT("Please enable world subsystem of Enhanced Input Developer Settings in Editor->Project Settings or the bind action of Actor would not be used"));

			return;
		}

		if (UMWWorldSubsystem* worldSubsys = Object->GetWorld()->GetSubsystem<UMWWorldSubsystem>())
		{
			if (AMWInputHandler* inputHandler = worldSubsys->GetInputHandler())
			{
				inputHandler->BindInputAction<UserClass>(Action, TriggerEvent, Object, Func, Params...);
			}
		}
	}

	template<DerivedFromUObject UserClass, typename FuncType, typename... VarTypes>
	static void BindInputAction(const UMWInputConfig* InputConfig, const FGameplayTag& InputActionTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func, VarTypes... Params)
	{
		if (!InputConfig)
		{
			UE_LOG(LogMWInput, Warning, TEXT("Input config is not set."));
			return;
		}

		if (const UInputAction* action = InputConfig->FindNativeInputActionForTag(InputActionTag))
		{
			UMWInputUtility::BindInputAction(action, TriggerEvent, Object, Func, Params...);
		}
		else
		{
			UE_LOG(LogMWInput, Warning, TEXT("InputActionTag[%s] does not bind an action."), *InputActionTag.ToString());
		}
	}

	static void ClearBindingsForObject(UObject* Object);

	static void RemoveBindingInputAction(const UObject* Object, const UInputAction* Action);

	static void RemoveBindingInputAction(const UObject* Object, const UMWInputConfig* InputConfig, const FGameplayTag& InputActionTag);

	/*
	 * Disable all input action in the tag container except the except tag.
	 * @Param Object       : Object binds the actions.
	 * @Param InputConfig  : The input config that owns the input actions.
	 * @Param TagContainer : The tag container of input actions to be disabled.
	 * @Param ExceptTag    : The input action tag that will not be disabled.
	*/
	static void DisableAllInputActionExcept(const UObject* Object, const UMWInputConfig* InputConfig, const TArray<FGameplayTag>& TagContainer, const FGameplayTag& ExceptTag);

	static void DisableAllInputAction(const UObject* Object, const UMWInputConfig* InputConfig, const TArray<FGameplayTag>& TagContainer);
};
