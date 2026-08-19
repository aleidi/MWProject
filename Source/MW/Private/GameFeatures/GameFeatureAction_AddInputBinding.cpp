#include "GameFeatures/GameFeatureAction_AddInputBinding.h"

//#include "Character/MWHeroComponent.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Controller/MWPlayerController.h"
#include "Engine/GameInstance.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "EnhancedInputSubsystems.h"
#include "GameFeatures/GameFeatureAction_WorldActionBase.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "Input/MWInputConfig.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

#include UE_INLINE_GENERATED_CPP_BY_NAME(GameFeatureAction_AddInputBinding)

#define LOCTEXT_NAMESPACE "GameFeatures"

//////////////////////////////////////////////////////////////////////
// UGameFeatureAction_AddInputBinding

void UGameFeatureAction_AddInputBinding::OnGameFeatureActivating(FGameFeatureActivatingContext& Context)
{
	FPerContextData& activeData = ContextData.FindOrAdd(Context);
	if (!ensure(activeData.ExtensionRequestHandles.IsEmpty()) || !ensure(activeData.PawnInputBindHandles.IsEmpty()))
	{
		Reset(activeData);
	}

	Super::OnGameFeatureActivating(Context);
}

void UGameFeatureAction_AddInputBinding::OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context)
{
	Super::OnGameFeatureDeactivating(Context);
	FPerContextData* activeData = ContextData.Find(Context);

	if (ensure(activeData))
	{
		Reset(*activeData);
	}
}

#if WITH_EDITOR
EDataValidationResult UGameFeatureAction_AddInputBinding::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult result = CombineDataValidationResults(Super::IsDataValid(Context), EDataValidationResult::Valid);

	int32 index = 0;

	for (const TSoftObjectPtr<const UMWInputConfig>& entry : InputConfigs)
	{
		if (entry.IsNull())
		{
			result = EDataValidationResult::Invalid;
			Context.AddError(FText::Format(LOCTEXT("NullInputConfig", "Null InputConfig at index {0}."), index));
		}
		++index;
	}

	return result;
}
#endif

void UGameFeatureAction_AddInputBinding::AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext)
{
	UWorld* world = WorldContext.World();
	UGameInstance* gameInstance = WorldContext.OwningGameInstance;
	FPerContextData& activeData = ContextData.FindOrAdd(ChangeContext);

	if ((gameInstance != nullptr) && (world != nullptr) && world->IsGameWorld())
	{
		if (UGameFrameworkComponentManager* componentManager = UGameInstance::GetSubsystem<UGameFrameworkComponentManager>(gameInstance))
		{
			UGameFrameworkComponentManager::FExtensionHandlerDelegate addAbilitiesDelegate =
				UGameFrameworkComponentManager::FExtensionHandlerDelegate::CreateUObject(this, &ThisClass::HandlePawnExtension, ChangeContext);
			TSharedPtr<FComponentRequestHandle> extensionRequestHandle =
				componentManager->AddExtensionHandler(APawn::StaticClass(), addAbilitiesDelegate);

			activeData.ExtensionRequestHandles.Add(extensionRequestHandle);
		}
	}
}

void UGameFeatureAction_AddInputBinding::Reset(FPerContextData& ActiveData)
{
	ActiveData.ExtensionRequestHandles.Empty();

	while (!ActiveData.PawnInputBindHandles.IsEmpty())
	{
		auto bindIt = ActiveData.PawnInputBindHandles.CreateIterator();
		RemoveInputMapping(bindIt->Key, ActiveData);
	}
}

void UGameFeatureAction_AddInputBinding::HandlePawnExtension(AActor* Actor, FName EventName, FGameFeatureStateChangeContext ChangeContext)
{
	APawn* asPawn = CastChecked<APawn>(Actor);
	FPerContextData& activeData = ContextData.FindOrAdd(ChangeContext);

	if ((EventName == UGameFrameworkComponentManager::NAME_ExtensionRemoved) || (EventName == UGameFrameworkComponentManager::NAME_ReceiverRemoved))
	{
		RemoveInputMapping(asPawn, activeData);
	}
	else if ((EventName == UGameFrameworkComponentManager::NAME_ExtensionAdded) /*|| (EventName == UMWHeroComponent::NAME_BindInputsNow)*/)
	{
		AddInputMappingForPlayer(asPawn, activeData);
	}
}

void UGameFeatureAction_AddInputBinding::AddInputMappingForPlayer(APawn* Pawn, FPerContextData& ActiveData)
{
	APlayerController* playerController = Cast<APlayerController>(Pawn->GetController());

	if (ULocalPlayer* localPlayer = playerController ? playerController->GetLocalPlayer() : nullptr)
	{
		if (UEnhancedInputLocalPlayerSubsystem* inputSystem = localPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (AMWPlayerController* mwPlayerController = Cast<AMWPlayerController>(playerController))
			{
				for (const TSoftObjectPtr<const UMWInputConfig>& entry : InputConfigs)
				{
					if (const UMWInputConfig* bindSet = entry.Get())
					{
						TArray<uint32>& inputBindHandles = ActiveData.PawnInputBindHandles.FindOrAdd(Pawn);

						TArray<uint32> outBindHandles;
						mwPlayerController->AddInputConfig(bindSet, outBindHandles);
						inputBindHandles.Append(outBindHandles);
					}
				}
			}
		}
		else
		{
			UE_LOG(LogGameFeatures, Error, TEXT("Failed to find `UEnhancedInputLocalPlayerSubsystem` for local player. Input mappings will not be added. Make sure you're set to use the EnhancedInput system via config file."));
		}
	}
}

void UGameFeatureAction_AddInputBinding::RemoveInputMapping(APawn* Pawn, FPerContextData& ActiveData)
{
	TArray<uint32>* bindHandles = ActiveData.PawnInputBindHandles.Find(Pawn);
	if (!bindHandles)
	{
		return;
	}

	if (APlayerController* playerController = Cast<APlayerController>(Pawn->GetController()))
	{
		if (ULocalPlayer* localPlayer = playerController->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* inputSystem = localPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				if (AMWPlayerController* mwPlayerController = Cast<AMWPlayerController>(playerController))
				{
					mwPlayerController->RemoveInputConfig(*bindHandles);
				}
			}
		}
	}

	// LocalPlayerまたはControllerがない場合のReset()無限ループを防ぐため、必ずマップから削除
	ActiveData.PawnInputBindHandles.Remove(Pawn);
}

#undef LOCTEXT_NAMESPACE

