#include "GameFeatures/GameFeatureAction_AddInputContextMapping.h"

//#include "Character/MWHeroComponent.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "EnhancedInputSubsystems.h"
#include "GameFeatures/GameFeatureAction_WorldActionBase.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "System/MWAssetManager.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

#include UE_INLINE_GENERATED_CPP_BY_NAME(GameFeatureAction_AddInputContextMapping)

#define LOCTEXT_NAMESPACE "GameFeatures"

//////////////////////////////////////////////////////////////////////
// UGameFeatureAction_AddInputContextMapping

void UGameFeatureAction_AddInputContextMapping::OnGameFeatureRegistering()
{
	Super::OnGameFeatureRegistering();

	RegisterInputMappingContexts();
}

void UGameFeatureAction_AddInputContextMapping::OnGameFeatureActivating(FGameFeatureActivatingContext& Context)
{
	FPerContextData& activeData = ContextData.FindOrAdd(Context);
	if (!ensure(activeData.ExtensionRequestHandles.IsEmpty()) ||
		!ensure(activeData.ControllersAddedTo.IsEmpty()))
	{
		Reset(activeData);
	}
	Super::OnGameFeatureActivating(Context);
}

void UGameFeatureAction_AddInputContextMapping::OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context)
{
	Super::OnGameFeatureDeactivating(Context);

	FPerContextData* activeData = ContextData.Find(Context);
	if (ensure(activeData))
	{
		Reset(*activeData);
	}
}

void UGameFeatureAction_AddInputContextMapping::OnGameFeatureUnregistering()
{
	Super::OnGameFeatureUnregistering();	
	
	UnregisterInputMappingContexts();
}

void UGameFeatureAction_AddInputContextMapping::RegisterInputMappingContexts()
{
	RegisterInputContextMappingsForGameInstanceHandle = FWorldDelegates::OnStartGameInstance.AddUObject(this, &UGameFeatureAction_AddInputContextMapping::RegisterInputContextMappingsForGameInstance);

	const TIndirectArray<FWorldContext>& worldContexts = GEngine->GetWorldContexts();
	for (TIndirectArray<FWorldContext>::TConstIterator worldContextIterator = worldContexts.CreateConstIterator(); worldContextIterator; ++worldContextIterator)
	{
		RegisterInputContextMappingsForGameInstance(worldContextIterator->OwningGameInstance);
	}
}

void UGameFeatureAction_AddInputContextMapping::RegisterInputContextMappingsForGameInstance(UGameInstance* GameInstance)
{
	if (GameInstance != nullptr && !GameInstance->OnLocalPlayerAddedEvent.IsBoundToObject(this))
	{
		GameInstance->OnLocalPlayerAddedEvent.AddUObject(this, &UGameFeatureAction_AddInputContextMapping::RegisterInputMappingContextsForLocalPlayer);
		GameInstance->OnLocalPlayerRemovedEvent.AddUObject(this, &UGameFeatureAction_AddInputContextMapping::UnregisterInputMappingContextsForLocalPlayer);
		
		for (TArray<ULocalPlayer*>::TConstIterator localPlayerIterator = GameInstance->GetLocalPlayerIterator(); localPlayerIterator; ++localPlayerIterator)
		{
			RegisterInputMappingContextsForLocalPlayer(*localPlayerIterator);
		}
	}
}

void UGameFeatureAction_AddInputContextMapping::RegisterInputMappingContextsForLocalPlayer(ULocalPlayer* LocalPlayer)
{
	if (ensure(LocalPlayer))
	{
		UMWAssetManager& assetManager = UMWAssetManager::Get();
		
		if (UEnhancedInputLocalPlayerSubsystem* eiSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
		{
			if (UEnhancedInputUserSettings* settings = eiSubsystem->GetUserSettings())
			{
				for (const FInputMappingContextAndPriority& entry : InputMappings)
				{
					// Skip entries that don't want to be registered
					if (!entry.bRegisterWithSettings)
					{
						continue;
					}

					// Register this IMC with the settings!
					if (UInputMappingContext* imc = assetManager.GetAsset(entry.InputMapping))
					{
						settings->RegisterInputMappingContext(imc);
					}
				}
			}
		}
	}
}

void UGameFeatureAction_AddInputContextMapping::UnregisterInputMappingContexts()
{
	FWorldDelegates::OnStartGameInstance.Remove(RegisterInputContextMappingsForGameInstanceHandle);
	RegisterInputContextMappingsForGameInstanceHandle.Reset();

	const TIndirectArray<FWorldContext>& worldContexts = GEngine->GetWorldContexts();
	for (TIndirectArray<FWorldContext>::TConstIterator worldContextIterator = worldContexts.CreateConstIterator(); worldContextIterator; ++worldContextIterator)
	{
		UnregisterInputContextMappingsForGameInstance(worldContextIterator->OwningGameInstance);
	}
}

void UGameFeatureAction_AddInputContextMapping::UnregisterInputContextMappingsForGameInstance(UGameInstance* GameInstance)
{
	if (GameInstance != nullptr)
	{
		GameInstance->OnLocalPlayerAddedEvent.RemoveAll(this);
		GameInstance->OnLocalPlayerRemovedEvent.RemoveAll(this);

		for (TArray<ULocalPlayer*>::TConstIterator localPlayerIterator = GameInstance->GetLocalPlayerIterator(); localPlayerIterator; ++localPlayerIterator)
		{
			UnregisterInputMappingContextsForLocalPlayer(*localPlayerIterator);
		}
	}
}

void UGameFeatureAction_AddInputContextMapping::UnregisterInputMappingContextsForLocalPlayer(ULocalPlayer* LocalPlayer)
{
	if (ensure(LocalPlayer))
	{
		if (UEnhancedInputLocalPlayerSubsystem* eiSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
		{
			if (UEnhancedInputUserSettings* settings = eiSubsystem->GetUserSettings())
			{
				for (const FInputMappingContextAndPriority& entry : InputMappings)
				{
					// Skip entries that don't want to be registered
					if (!entry.bRegisterWithSettings)
					{
						continue;
					}

					// Register this IMC with the settings!
					if (UInputMappingContext* imc = entry.InputMapping.Get())
					{
						settings->UnregisterInputMappingContext(imc);
					}
				}
			}
		}
	}
}


#if WITH_EDITOR
EDataValidationResult UGameFeatureAction_AddInputContextMapping::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult result = CombineDataValidationResults(Super::IsDataValid(Context), EDataValidationResult::Valid);

	int32 index = 0;

	for (const FInputMappingContextAndPriority& entry : InputMappings)
	{
		if (entry.InputMapping.IsNull())
		{
			result = EDataValidationResult::Invalid;
			Context.AddError(FText::Format(LOCTEXT("NullInputMapping", "Null InputMapping at index {0}."), index));
		}
		++index;
	}

	return result;
}
#endif

void UGameFeatureAction_AddInputContextMapping::AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext)
{
	UWorld* world = WorldContext.World();
	UGameInstance* gameInstance = WorldContext.OwningGameInstance;
	FPerContextData& activeData = ContextData.FindOrAdd(ChangeContext);

	if ((gameInstance != nullptr) && (world != nullptr) && world->IsGameWorld())
	{
		if (UGameFrameworkComponentManager* componentManager = UGameInstance::GetSubsystem<UGameFrameworkComponentManager>(gameInstance))
		{
			UGameFrameworkComponentManager::FExtensionHandlerDelegate addAbilitiesDelegate =
				UGameFrameworkComponentManager::FExtensionHandlerDelegate::CreateUObject(this, &ThisClass::HandleControllerExtension, ChangeContext);
			TSharedPtr<FComponentRequestHandle> extensionRequestHandle =
				componentManager->AddExtensionHandler(APlayerController::StaticClass(), addAbilitiesDelegate);

			activeData.ExtensionRequestHandles.Add(extensionRequestHandle);
		}
	}
}

void UGameFeatureAction_AddInputContextMapping::Reset(FPerContextData& ActiveData)
{
	ActiveData.ExtensionRequestHandles.Empty();

	while (!ActiveData.ControllersAddedTo.IsEmpty())
	{
		TWeakObjectPtr<APlayerController> controllerPtr = ActiveData.ControllersAddedTo.Top();
		if (controllerPtr.IsValid())
		{
			RemoveInputMapping(controllerPtr.Get(), ActiveData);
		}
		else
		{
			ActiveData.ControllersAddedTo.Pop();
		}
	}
}

void UGameFeatureAction_AddInputContextMapping::HandleControllerExtension(AActor* Actor, FName EventName, FGameFeatureStateChangeContext ChangeContext)
{
	APlayerController* asController = CastChecked<APlayerController>(Actor);
	FPerContextData& activeData = ContextData.FindOrAdd(ChangeContext);

	// TODO Why does this code mix and match controllers and local players? ControllersAddedTo is never modified
	if ((EventName == UGameFrameworkComponentManager::NAME_ExtensionRemoved) || (EventName == UGameFrameworkComponentManager::NAME_ReceiverRemoved))
	{
		RemoveInputMapping(asController, activeData);
	}
	else if ((EventName == UGameFrameworkComponentManager::NAME_ExtensionAdded)/* || (EventName == UMWHeroComponent::NAME_BindInputsNow)*/)
	{
		AddInputMappingForPlayer(asController->GetLocalPlayer(), activeData);
	}
}

void UGameFeatureAction_AddInputContextMapping::AddInputMappingForPlayer(UPlayer* Player, FPerContextData& ActiveData)
{
	if (ULocalPlayer* localPlayer = Cast<ULocalPlayer>(Player))
	{
		if (UEnhancedInputLocalPlayerSubsystem* inputSystem = localPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			for (const FInputMappingContextAndPriority& entry : InputMappings)
			{
				if (const UInputMappingContext* imc = entry.InputMapping.Get())
				{
					inputSystem->AddMappingContext(imc, entry.Priority);
				}
			}
		}
		else
		{
			UE_LOG(LogGameFeatures, Error, TEXT("Failed to find `UEnhancedInputLocalPlayerSubsystem` for local player. Input mappings will not be added. Make sure you're set to use the EnhancedInput system via config file."));
		}
	}
}

void UGameFeatureAction_AddInputContextMapping::RemoveInputMapping(APlayerController* PlayerController, FPerContextData& ActiveData)
{
	if (ULocalPlayer* localPlayer = PlayerController->GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* inputSystem = localPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			for (const FInputMappingContextAndPriority& entry : InputMappings)
			{
				if (const UInputMappingContext* imc = entry.InputMapping.Get())
				{
					inputSystem->RemoveMappingContext(imc);
				}
			}
		}
	}

	ActiveData.ControllersAddedTo.Remove(PlayerController);
}

#undef LOCTEXT_NAMESPACE
