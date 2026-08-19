#include "System/MWAssetManager.h"

#include "Data/MWGameplayData.h"
#include "Engine/Engine.h"
#include "Gameplay/MWGameplayTags.h"
#include "Misc/ScopedSlowTask.h"
#include "MWLogChannels.h"
#include "Stats/StatsMisc.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MWAssetManager)

const FName FMWBundles::Equipped("Equipped");

//////////////////////////////////////////////////////////////////////

static FAutoConsoleCommand CVarDumpLoadedAssets(
	TEXT("MW.DumpLoadedAssets"),
	TEXT("Shows all assets that were loaded via the asset manager and are currently in memory."),
	FConsoleCommandDelegate::CreateStatic(UMWAssetManager::DumpLoadedAssets)
);

//////////////////////////////////////////////////////////////////////

UMWAssetManager::UMWAssetManager()
{
}

UMWAssetManager& UMWAssetManager::Get()
{
	check(GEngine);

	if (UMWAssetManager* Singleton = Cast<UMWAssetManager>(GEngine->AssetManager))
	{
		return *Singleton;
	}

	UE_LOG(LogMW, Fatal, TEXT("Invalid AssetManagerClassName in DefaultEngine.ini.  It must be set to MWAssetManager!"));

	// 上記のFatal Errorによりここには到達しない
	return *NewObject<UMWAssetManager>();
}

UObject* UMWAssetManager::SynchronousLoadAsset(const FSoftObjectPath& AssetPath)
{
	if (AssetPath.IsValid())
	{
		TUniquePtr<FScopeLogTime> LogTimePtr;

		if (ShouldLogAssetLoads())
		{
			LogTimePtr = MakeUnique<FScopeLogTime>(*FString::Printf(TEXT("Synchronously loaded asset [%s]"), *AssetPath.ToString()), nullptr, FScopeLogTime::ScopeLog_Seconds);
		}

		if (UAssetManager::IsInitialized())
		{
			return UAssetManager::GetStreamableManager().LoadSynchronous(AssetPath, false);
		}

		// Asset Managerが未準備ならLoadObjectを使用
		return AssetPath.TryLoad();
	}

	return nullptr;
}

bool UMWAssetManager::ShouldLogAssetLoads()
{
	static bool bLogAssetLoads = FParse::Param(FCommandLine::Get(), TEXT("LogAssetLoads"));
	return bLogAssetLoads;
}

void UMWAssetManager::AddLoadedAsset(const UObject* Asset)
{
	if (ensureAlways(Asset))
	{
		FScopeLock LoadedAssetsLock(&LoadedAssetsCritical);
		LoadedAssets.Add(Asset);
	}
}

void UMWAssetManager::DumpLoadedAssets()
{
	UE_LOG(LogMW, Log, TEXT("========== Start Dumping Loaded Assets =========="));

	for (const UObject* LoadedAsset : Get().LoadedAssets)
	{
		UE_LOG(LogMW, Log, TEXT("  %s"), *GetNameSafe(LoadedAsset));
	}

	UE_LOG(LogMW, Log, TEXT("... %d assets in loaded pool"), Get().LoadedAssets.Num());
	UE_LOG(LogMW, Log, TEXT("========== Finish Dumping Loaded Assets =========="));
}

void UMWAssetManager::StartInitialLoading()
{
	SCOPED_BOOT_TIMING("UMWAssetManager::StartInitialLoading");

	// 全スキャンを実行するため、ロードを遅延する場合もここで処理
	Super::StartInitialLoading();
}

UPrimaryDataAsset* UMWAssetManager::LoadGameDataOfClass(TSubclassOf<UPrimaryDataAsset> DataClass, const TSoftObjectPtr<UPrimaryDataAsset>& DataClassPath, FPrimaryAssetType PrimaryAssetType)
{
	UPrimaryDataAsset* Asset = nullptr;

	//DECLARE_SCOPE_CYCLE_COUNTER(TEXT("Loading GameData Object"), STAT_GameData, STATGROUP_LoadTime);
	if (!DataClassPath.IsNull())
	{
#if WITH_EDITOR
		FScopedSlowTask SlowTask(0, FText::Format(NSLOCTEXT("MWEditor", "BeginLoadingGameDataTask", "Loading GameData {0}"), FText::FromName(DataClass->GetFName())));
		const bool bShowCancelButton = false;
		const bool bAllowInPIE = true;
		SlowTask.MakeDialog(bShowCancelButton, bAllowInPIE);
#endif
		UE_LOG(LogMW, Log, TEXT("Loading GameData: %s ..."), *DataClassPath.ToString());
		SCOPE_LOG_TIME_IN_SECONDS(TEXT("    ... GameData loaded!"), nullptr);

		// EditorではPostLoadから再帰呼び出しされ得るため、Primary Assetを同期ロードし、残りを非同期ロード
		if (GIsEditor)
		{
			Asset = DataClassPath.LoadSynchronous();
			LoadPrimaryAssetsWithType(PrimaryAssetType);
		}
		else
		{
			TSharedPtr<FStreamableHandle> Handle = LoadPrimaryAssetsWithType(PrimaryAssetType);
			if (Handle.IsValid())
			{
				Handle->WaitUntilComplete(0.0f, false);

				// 常に成功する想定
				Asset = Cast<UPrimaryDataAsset>(Handle->GetLoadedAsset());
			}
		}
	}

	if (Asset)
	{
		GameDataMap.Add(DataClass, Asset);
	}
	else
	{
		// GameDataのロード失敗は診断困難なサイレント障害を招くため許容しない
		UE_LOG(LogMW, Fatal, TEXT("Failed to load GameData asset at %s. Type %s. This is not recoverable and likely means you do not have the correct data to run %s."), *DataClassPath.ToString(), *PrimaryAssetType.ToString(), FApp::GetProjectName());
	}

	return Asset;
}

#if WITH_EDITOR
void UMWAssetManager::PreBeginPIE(bool bStartSimulate)
{
	Super::PreBeginPIE(bStartSimulate);

	{
		FScopedSlowTask SlowTask(0, NSLOCTEXT("MWEditor", "BeginLoadingPIEData", "Loading PIE Data"));
		const bool bShowCancelButton = false;
		const bool bAllowInPIE = true;
		SlowTask.MakeDialog(bShowCancelButton, bAllowInPIE);

		//const UMWGameData& LocalGameDataCommon = GetGameData();

		// GameDataの処理時間を計測対象外にするためGetGameData後に実行
		SCOPE_LOG_TIME_IN_SECONDS(TEXT("PreBeginPIE asset preloading complete"), nullptr);

		// 使用するExperienceに必要な追加アセットをここで事前読み込み可能
		// 例: World SettingsのデフォルトExperienceとDeveloper SettingsのOverride
	}
}
#endif
