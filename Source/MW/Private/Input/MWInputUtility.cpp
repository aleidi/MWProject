#include "Input/MWInputUtility.h"
#include "Data/MWMasterData.h"
#include "EnhancedInputSubsystems.h"
#include "Gameplay/MWGameplayTags.h"
#include "Input/MWInputComponent.h"
#include "Input/MWInputConfig.h"
#include "MWGameSingleton.h"

void UMWInputUtility::EnableMappingContext(APlayerController* PC, const FGameplayTag& Tag, const FModifyContextOptions& MappingOption)
{
	if (!PC)
	{
		return;
	}

	const ULocalPlayer* localPlayer = Cast<ULocalPlayer>(PC->GetLocalPlayer());
	check(localPlayer);

	UEnhancedInputLocalPlayerSubsystem* subsystem = localPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(subsystem);

	const UMWMasterData* data = MWSINGLETON->GetMasterData();
	{
		if (UMWInputConfig* inputConfig = data->InputConfig.Get())
		{
			if (UEnhancedInputUserSettings* settings = subsystem->GetUserSettings())
			{
				if (FMWInputMappingContextWithPriority* mapping = inputConfig->InputMappingContext.Find(Tag))
				{
					if (mapping->Mapping.Get())
					{
						//settings->RegisterInputMappingContext(mapping->Mapping);

						subsystem->AddMappingContext(mapping->Mapping, mapping->Priority, MappingOption);
					}
				}
			}
		}
	}
}

void UMWInputUtility::DisableMappingContext(APlayerController* PC, const FGameplayTag& Tag, const FModifyContextOptions& MappingOption)
{
	if (!PC)
	{
		return;
	}

	const ULocalPlayer* localPlayer = Cast<ULocalPlayer>(PC->GetLocalPlayer());
	check(localPlayer);

	UEnhancedInputLocalPlayerSubsystem* subsystem = localPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(subsystem);

	const UMWMasterData* data = MWSINGLETON->GetMasterData();
	{
		if (UMWInputConfig* inputConfig = data->InputConfig.Get())
		{
			if (UEnhancedInputUserSettings* settings = subsystem->GetUserSettings())
			{
				if (FMWInputMappingContextWithPriority* mapping = inputConfig->InputMappingContext.Find(Tag))
				{
					if (mapping->Mapping.Get())
					{
						subsystem->RemoveMappingContext(mapping->Mapping, MappingOption);
					}
				}
			}
		}
	}
}

void UMWInputUtility::ClearBindingsForObject(UObject* Object)
{
	if (!Object || !Object->GetWorld())
	{
		return;
	}

	if (UMWWorldSubsystem* worldSubsys = Object->GetWorld()->GetSubsystem<UMWWorldSubsystem>())
	{
		if (AMWInputHandler* inputHandler = worldSubsys->GetInputHandler())
		{
			inputHandler->ClearBindingsForObject(Object);
		}
	}
}

void UMWInputUtility::RemoveBindingInputAction(const UObject* Object, const UInputAction* Action)
{
	if (!Object && !Object->GetWorld() && !Action)
	{
		return;
	}

	if (UMWWorldSubsystem* worldSubsys = Object->GetWorld()->GetSubsystem<UMWWorldSubsystem>())
	{
		if (AMWInputHandler* inputHandler = worldSubsys->GetInputHandler())
		{
			inputHandler->RemoveBindingInputAction(Object, Action);
		}
	}
}

void UMWInputUtility::RemoveBindingInputAction(const UObject* Object, const FGameplayTag& IMCTag, const FGameplayTag& InputActionTag)
{
	const UMWMasterData* data = MWSINGLETON->GetMasterData();
	if (UMWInputConfig* input_config = data->InputConfig.Get())
	{
		if (const UInputAction* action = input_config->FindNativeInputActionForTag(IMCTag, InputActionTag))
		{
			UMWInputUtility::RemoveBindingInputAction(Object, action);
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

void UMWInputUtility::DisableAllInputActionExcept(const UObject* Object, const FGameplayTag& IMCTag, const TArray<FGameplayTag>& TagContainer, const FGameplayTag& ExceptTag)
{
	for (const auto& tag : TagContainer)
	{
		if (tag != ExceptTag)
		{
			UMWInputUtility::RemoveBindingInputAction(Object, IMCTag, tag);
		}
	}
}

void UMWInputUtility::DisableAllInputAction(const UObject* Object, const FGameplayTag& IMCTag, const TArray<FGameplayTag>& TagContainer)
{
	for (const auto& tag : TagContainer)
	{
		UMWInputUtility::RemoveBindingInputAction(Object, IMCTag, tag);
	}
}
