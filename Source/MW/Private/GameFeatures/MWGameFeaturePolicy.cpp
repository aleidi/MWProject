// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameFeatures/MWGameFeaturePolicy.h"

//#include "AbilitySystem/MWGameplayCueManager.h"
#include "GameFeatureData.h"
#include "GameplayCueSet.h"

UMWGameFeaturePolicy::UMWGameFeaturePolicy(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UMWGameFeaturePolicy& UMWGameFeaturePolicy::Get()
{
	return UGameFeaturesSubsystem::Get().GetPolicy<UMWGameFeaturePolicy>();
}

void UMWGameFeaturePolicy::InitGameFeatureManager()
{
	Observers.Add(NewObject<UMWGameFeature_HotfixManager>());
	Observers.Add(NewObject<UMWGameFeature_AddGameplayCuePaths>());

	UGameFeaturesSubsystem& Subsystem = UGameFeaturesSubsystem::Get();
	for (UObject* Observer : Observers)
	{
		Subsystem.AddObserver(Observer, UGameFeaturesSubsystem::EObserverPluginStateUpdateMode::CurrentAndFuture);
	}

	Super::InitGameFeatureManager();
}

void UMWGameFeaturePolicy::ShutdownGameFeatureManager()
{
	Super::ShutdownGameFeatureManager();

	UGameFeaturesSubsystem& Subsystem = UGameFeaturesSubsystem::Get();
	for (UObject* Observer : Observers)
	{
		Subsystem.RemoveObserver(Observer);
	}
	Observers.Empty();
}

TArray<FPrimaryAssetId> UMWGameFeaturePolicy::GetPreloadAssetListForGameFeature(const UGameFeatureData* GameFeatureToLoad, bool bIncludeLoadedAssets) const
{
	return Super::GetPreloadAssetListForGameFeature(GameFeatureToLoad, bIncludeLoadedAssets);
}

const TArray<FName> UMWGameFeaturePolicy::GetPreloadBundleStateForGameFeature() const
{
	return Super::GetPreloadBundleStateForGameFeature();
}

void UMWGameFeaturePolicy::GetGameFeatureLoadingMode(bool& bLoadClientData, bool& bLoadServerData) const
{
	// Editor will load both, this can cause hitching as the bundles are set to not preload in editor
	bLoadClientData = !IsRunningDedicatedServer();
	bLoadServerData = !IsRunningClientOnly();
}

bool UMWGameFeaturePolicy::IsPluginAllowed(const FString& PluginURL, FString* OutReason) const
{
	return Super::IsPluginAllowed(PluginURL, OutReason);
}

//////////////////////////////////////////////////////////////////////
//

//#include "Hotfix/MWHotfixManager.h"

void UMWGameFeature_HotfixManager::OnGameFeatureLoading(const UGameFeatureData* GameFeatureData, const FString& PluginURL)
{
	/*if (UMWHotfixManager* HotfixManager = Cast<UMWHotfixManager>(UOnlineHotfixManager::Get(nullptr)))
	{
		HotfixManager->RequestPatchAssetsFromIniFiles();
	}*/
}

//////////////////////////////////////////////////////////////////////
//

#include "AbilitySystemGlobals.h"
//#include "GameFeatureAction_AddGameplayCuePath.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MWGameFeaturePolicy)

class FName;
struct FPrimaryAssetId;

void UMWGameFeature_AddGameplayCuePaths::OnGameFeatureRegistering(const UGameFeatureData* GameFeatureData, const FString& PluginName, const FString& PluginURL)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(UMWGameFeature_AddGameplayCuePaths::OnGameFeatureRegistering);
	
	const FString PluginRootPath = TEXT("/") + PluginName;
	for (const UGameFeatureAction* Action : GameFeatureData->GetActions())
	{
		//if (const UGameFeatureAction_AddGameplayCuePath* AddGameplayCueGFA = Cast<UGameFeatureAction_AddGameplayCuePath>(Action))
		//{
		//	const TArray<FDirectoryPath>& DirsToAdd = AddGameplayCueGFA->GetDirectoryPathsToAdd();
		//	
		//	if (UMWGameplayCueManager* GCM = UMWGameplayCueManager::Get())
		//	{
		//		UGameplayCueSet* RuntimeGameplayCueSet = GCM->GetRuntimeCueSet();
		//		const int32 PreInitializeNumCues = RuntimeGameplayCueSet ? RuntimeGameplayCueSet->GameplayCueData.Num() : 0;

		//		for (const FDirectoryPath& Directory : DirsToAdd)
		//		{
		//			FString MutablePath = Directory.Path;
		//			UGameFeaturesSubsystem::FixPluginPackagePath(MutablePath, PluginRootPath, false);
		//			GCM->AddGameplayCueNotifyPath(MutablePath, /** bShouldRescanCueAssets = */ false);	
		//		}
		//		
		//		// Rebuild the runtime library with these new paths
		//		if (!DirsToAdd.IsEmpty())
		//		{
		//			GCM->InitializeRuntimeObjectLibrary();	
		//		}

		//		const int32 PostInitializeNumCues = RuntimeGameplayCueSet ? RuntimeGameplayCueSet->GameplayCueData.Num() : 0;
		//		if (PreInitializeNumCues != PostInitializeNumCues)
		//		{
		//			GCM->RefreshGameplayCuePrimaryAsset();
		//		}
		//	}
		//}
	}
}

void UMWGameFeature_AddGameplayCuePaths::OnGameFeatureUnregistering(const UGameFeatureData* GameFeatureData, const FString& PluginName, const FString& PluginURL)
{
	const FString PluginRootPath = TEXT("/") + PluginName;
	for (const UGameFeatureAction* Action : GameFeatureData->GetActions())
	{
		//if (const UGameFeatureAction_AddGameplayCuePath* AddGameplayCueGFA = Cast<UGameFeatureAction_AddGameplayCuePath>(Action))
		//{
		//	const TArray<FDirectoryPath>& DirsToAdd = AddGameplayCueGFA->GetDirectoryPathsToAdd();
		//	
		//	if (UGameplayCueManager* GCM = UAbilitySystemGlobals::Get().GetGameplayCueManager())
		//	{
		//		int32 NumRemoved = 0;
		//		for (const FDirectoryPath& Directory : DirsToAdd)
		//		{
		//			FString MutablePath = Directory.Path;
		//			UGameFeaturesSubsystem::FixPluginPackagePath(MutablePath, PluginRootPath, false);
		//			NumRemoved += GCM->RemoveGameplayCueNotifyPath(MutablePath, /** bShouldRescanCueAssets = */ false);
		//		}

		//		ensure(NumRemoved == DirsToAdd.Num());
		//		
		//		// Rebuild the runtime library only if there is a need to
		//		if (NumRemoved > 0)
		//		{
		//			GCM->InitializeRuntimeObjectLibrary();	
		//		}			
		//	}
		//}
	}
}
