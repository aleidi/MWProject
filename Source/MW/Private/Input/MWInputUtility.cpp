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
			if (FMWInputMappingContextWithPriority* mapping = inputConfig->InputMappingContext.Find(Tag))
			{
				if (mapping->Mapping.Get())
				{
					subsystem->AddMappingContext(mapping->Mapping, mapping->Priority, MappingOption);
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
	if (const UInputAction* action = GetInputActionByTag(IMCTag, InputActionTag))
	{
		UMWInputUtility::RemoveBindingInputAction(Object, action);
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

bool UMWInputUtility::RemoveBindingInputActionByHandle(const UObject* Object, uint32 Handle)
{
	if (!Object || !Object->GetWorld() || Handle == 0)
	{
		return false;
	}

	if (UMWWorldSubsystem* worldSubsys = Object->GetWorld()->GetSubsystem<UMWWorldSubsystem>())
	{
		if (AMWInputHandler* inputHandler = worldSubsys->GetInputHandler())
		{
			return inputHandler->RemoveBindingInputAction(Handle);
		}
	}

	return false;
}

const UInputAction* UMWInputUtility::GetInputActionByTag(const FGameplayTag& IMCTag, const FGameplayTag& InputActionTag)
{
	const UMWMasterData* data = MWSINGLETON->GetMasterData();

	if (UMWInputConfig* input_config = data->InputConfig.Get())
	{
		if (const UInputAction* action = input_config->FindInputActionForTag(IMCTag, InputActionTag))
		{
			return action;
		}

		UE_LOG(LogMWInput, Warning, TEXT("InputActionTag[%s] does not bind an action."), *InputActionTag.ToString());

		return  nullptr;
	}

	UE_LOG(LogMWInput, Warning, TEXT("Input config is not set."));

	return nullptr;
}
