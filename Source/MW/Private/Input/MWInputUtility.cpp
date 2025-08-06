#include "Input/MWInputUtility.h"
#include "EnhancedInputSubsystems.h"
#include "Input/MWInputConfig.h"
#include "Data/MWMasterData.h"
#include "System/MWAssetManager.h"

void UMWInputUtility::EnableMappingContext(APlayerController* PC, const FName& Tag, const FModifyContextOptions& MappingOption)
{
	if (!PC)
	{
		return;
	}

	const ULocalPlayer* localPlayer = Cast<ULocalPlayer>(PC->GetLocalPlayer());
	check(localPlayer);

	UEnhancedInputLocalPlayerSubsystem* subsystem = localPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(subsystem);

	const UMWMasterData& data = UMWAssetManager::Get().GetMasterData();
	{
		if (UMWInputConfig* inputConfig = data.InputConfig.Get())
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

void UMWInputUtility::DisableMappingContext(APlayerController* PC, const FName& Tag, const FModifyContextOptions& MappingOption)
{
	if (!PC)
	{
		return;
	}

	const ULocalPlayer* localPlayer = Cast<ULocalPlayer>(PC->GetLocalPlayer());
	check(localPlayer);

	UEnhancedInputLocalPlayerSubsystem* subsystem = localPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(subsystem);

	const UMWMasterData& data = UMWAssetManager::Get().GetMasterData();
	{
		if (UMWInputConfig* inputConfig = data.InputConfig.Get())
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
