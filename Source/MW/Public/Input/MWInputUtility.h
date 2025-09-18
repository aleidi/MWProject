#pragma once

// Include
#include "CoreMinimal.h"
#include "EnhancedInputSubsystemInterface.h"
#include "MWLogChannels.h"
#include "EnhancedInputDeveloperSettings.h"
#include "EnhancedInputComponent.h"
#include "System/MWWorldSubsystem.h"
#include "MWInputHandler.h"
#include "System/MWAssetManager.h"
#include "MWLogChannels.h"
#include "Data/MWMasterData.h"
#include "Define/MWDefineCommon.h"
#include "MWInputUtility.generated.h"

// Forward Declare

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
	UFUNCTION(BlueprintCallable)
	static void EnableMappingContext(APlayerController* PC, const FGameplayTag& Tag, const FModifyContextOptions& Options = FModifyContextOptions());

	UFUNCTION(BlueprintCallable)
	static void DisableMappingContext(APlayerController* PC, const FGameplayTag& Tag, const FModifyContextOptions& Options = FModifyContextOptions());

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
	static void BindInputAction(const FGameplayTag& InputActionTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func, VarTypes... Params)
	{
		const UMWMasterData& data = UMWAssetManager::Get().GetMasterData();

		if (UMWInputConfig* input_config = data.InputConfig.Get())
		{
			if (const UInputAction* action = input_config->FindNativeInputActionForTag(InputActionTag, false))
			{
				UMWInputUtility::BindInputAction(action, TriggerEvent, Object, Func, Params...);
			}
			else
			{
				UE_LOG(LogMWInput, Warning, TEXT("InputActionTag[%s] does not bind an action."), *InputActionTag.ToString());
			}
		}
		else
		{
			UE_LOG(LogMWInput, Warning, TEXT("Input config is not set."));
		}
	}

	static void ClearBindingsForObject(UObject* Object);

	static void RemoveBindingInputAction(const UObject* Object, const UInputAction* Action);

	static void RemoveBindingInputAction(const UObject* Object, const FGameplayTag& InputActionTag);
};
