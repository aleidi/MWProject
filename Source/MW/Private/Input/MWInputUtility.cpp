#include "Input/MWInputUtility.h"

#include "Data/MWGameplayData.h"
#include "EnhancedInputSubsystems.h"
#include "Gameplay/MWGameplayTags.h"
#include "Input/MWInputComponent.h"
#include "Input/MWInputConfig.h"
#include "InputMappingContext.h"
#include "MWGameSingleton.h"
#include "MWLogChannels.h"

void UMWInputUtility::EnableMappingContext(APlayerController* PC, const UMWInputConfig* InputConfig, const FModifyContextOptions& MappingOption)
{
	if (!PC || !InputConfig)
	{
		return;
	}

	const ULocalPlayer* localPlayer = Cast<ULocalPlayer>(PC->GetLocalPlayer());
	check(localPlayer);

	UEnhancedInputLocalPlayerSubsystem* subsystem = localPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(subsystem);

	const FMWInputMappingContextWithPriority& mapping = InputConfig->InputMappingContext;
	if (mapping.Mapping.Get())
	{
		UE_LOG(LogMWInput, Display, TEXT("EnableMappingContext: Add [%s] priority=%d  PC=[%s]  InputConfig=[%s]"), *mapping.Mapping->GetName(), mapping.Priority, *PC->GetName(), *InputConfig->GetName());

		subsystem->AddMappingContext(mapping.Mapping, mapping.Priority, MappingOption);
	}
}

void UMWInputUtility::DisableMappingContext(APlayerController* PC, const UMWInputConfig* InputConfig, const FModifyContextOptions& MappingOption)
{
	if (!PC || !InputConfig)
	{
		return;
	}

	const ULocalPlayer* localPlayer = Cast<ULocalPlayer>(PC->GetLocalPlayer());
	check(localPlayer);

	UEnhancedInputLocalPlayerSubsystem* subsystem = localPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(subsystem);

	const FMWInputMappingContextWithPriority& mapping = InputConfig->InputMappingContext;
	if (mapping.Mapping.Get())
	{
		UE_LOG(LogMWInput, Display, TEXT("EnableMappingContext: Remove [%s] priority=%d  PC=[%s]  InputConfig=[%s]"), *mapping.Mapping->GetName(), mapping.Priority, *PC->GetName(), *InputConfig->GetName());

		subsystem->RemoveMappingContext(mapping.Mapping, MappingOption);
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

void UMWInputUtility::RemoveBindingInputAction(const UObject* Object, const UMWInputConfig* InputConfig, const FGameplayTag& InputActionTag)
{
	if (!InputConfig)
	{
		UE_LOG(LogMWInput, Warning, TEXT("Input config is not set."));
		return;
	}

	if (const UInputAction* action = InputConfig->FindNativeInputActionForTag(InputActionTag))
	{
		UMWInputUtility::RemoveBindingInputAction(Object, action);
	}
	else
	{
		UE_LOG(LogMWInput, Warning, TEXT("InputActionTag[%s] does not bind an action."), *InputActionTag.ToString());
	}
}

void UMWInputUtility::DisableAllInputActionExcept(const UObject* Object, const UMWInputConfig* InputConfig, const TArray<FGameplayTag>& TagContainer, const FGameplayTag& ExceptTag)
{
	for (const auto& tag : TagContainer)
	{
		if (tag != ExceptTag)
		{
			UMWInputUtility::RemoveBindingInputAction(Object, InputConfig, tag);
		}
	}
}

void UMWInputUtility::DisableAllInputAction(const UObject* Object, const UMWInputConfig* InputConfig, const TArray<FGameplayTag>& TagContainer)
{
	for (const auto& tag : TagContainer)
	{
		UMWInputUtility::RemoveBindingInputAction(Object, InputConfig, tag);
	}
}
