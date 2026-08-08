#include "UI/ViewModel/MWViewModelManager.h"

#include "MVVMGameSubsystem.h"
#include "MVVMViewModelBase.h"
#include "MWLogChannels.h"
#include "System/MWGameInstanceSubsystem.h"
#include "Types/MVVMViewModelContext.h"
#include "UI/ViewModel/MWMVVMContextNames.h"

void UMWViewModelManager::Initialize()
{
	GlobalViewModelCollection = nullptr;

	if (UMVVMGameSubsystem* mvvmGameSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UMVVMGameSubsystem>())
	{
		GlobalViewModelCollection = mvvmGameSubsystem->GetViewModelCollection();
	}

	checkf(IsValid(GlobalViewModelCollection), TEXT("UMWViewModelManager::Initialize failed: GlobalViewModelCollection is null."));

	RegisterDefaultGlobalViewModels();
}

UMWViewModelManager* UMWViewModelManager::Get(const UObject* WorldContext)
{
	if(UMWGameInstanceSubsystem* subsystem = UMWGameInstanceSubsystem::Get(WorldContext))
	{
		return subsystem->GetViewModelManager();
	}

	return nullptr;
}

UMVVMViewModelBase* UMWViewModelManager::RegisterGlobalViewModel(FName ViewModelName, TSubclassOf<UMVVMViewModelBase> ViewModelClass, bool bForce)
{
	if (ViewModelName.IsNone() || !IsValid(ViewModelClass.Get()) || !IsValid(GlobalViewModelCollection))
	{
		return nullptr;
	}

	FMVVMViewModelContext context;
	context.ContextClass = ViewModelClass;
	context.ContextName = ViewModelName;

	if(UMVVMViewModelBase* existedViewModel = GlobalViewModelCollection->FindViewModelInstance(context))
	{
		if(!bForce)
		{
			return existedViewModel;
		}

		GlobalViewModelCollection->RemoveViewModel(context);
	}

	UMVVMViewModelBase* newViewModel = NewObject<UMVVMViewModelBase>(this, ViewModelClass);
	if (!newViewModel)
	{
		return nullptr;
	}

	if (!GlobalViewModelCollection->AddViewModelInstance(context, newViewModel))
	{
		return nullptr;
	}

	return newViewModel;
}

UMVVMViewModelBase* UMWViewModelManager::FindGlobalViewModel(FName ViewModelName, TSubclassOf<UMVVMViewModelBase> ViewModelClass) const
{
	if (ViewModelName.IsNone() || !IsValid(ViewModelClass.Get()) || !IsValid(GlobalViewModelCollection))
	{
		return nullptr;
	}

	FMVVMViewModelContext context;
	context.ContextClass = ViewModelClass;
	context.ContextName = ViewModelName;

	return GlobalViewModelCollection->FindViewModelInstance(context);
}

void UMWViewModelManager::RegisterDefaultGlobalViewModels()
{
	const TArray<MWMVVMContextNames::FGlobalViewModelDefinition>& definitions = MWMVVMContextNames::GetGlobalDefinitions();

	for(const MWMVVMContextNames::FGlobalViewModelDefinition& definition : definitions)
	{
		UMVVMViewModelBase* viewModel = RegisterGlobalViewModel(definition.ContextName, definition.ViewModelClass);

		if(!viewModel)
		{
			UE_LOG(LogMWUI, Warning, TEXT("Failed to register global view model: %s"), *definition.ContextName.ToString());

			continue;
		}
	}
}
