#include "Data/Skill/MWSkillDataManager.h"

#include "Data/Skill/MWSkillAsset.h"
#include "Engine/AssetManager.h"
#include "Engine/DataTable.h"
#include "MWLogChannels.h"
#include "Settings/MWGameSettings.h"
#include "System/MWGameInstanceSubsystem.h"

void UMWSkillDataManager::Initialize()
{
	SkillRowById.Reset();

	if (!LoadSkillTableFromConfig())
	{
		return;
	}

	RebuildSkillTableCache();
}

void UMWSkillDataManager::Deinitialize()
{
	SkillRowById.Reset();
	SkillTable = nullptr;
}

UMWSkillDataManager* UMWSkillDataManager::Get(const UObject* WorldContext)
{
	if (UMWGameInstanceSubsystem* subsystem = UMWGameInstanceSubsystem::Get(WorldContext))
	{
		return subsystem->GetSkillDataManager();
	}

	return nullptr;
}

const FMWSkillTable* UMWSkillDataManager::FindSkillRow(int32 SkillId) const
{
	return SkillRowById.Find(SkillId);
}

bool UMWSkillDataManager::TryGetSkillRow(int32 SkillId, FMWSkillTable& OutRow) const
{
	if (const FMWSkillTable* row = FindSkillRow(SkillId))
	{
		OutRow = *row;
		return true;
	}

	return false;
}

TSoftObjectPtr<UMWSkillAsset> UMWSkillDataManager::GetSkillAssetRef(int32 SkillId) const
{
	if (const FMWSkillTable* row = FindSkillRow(SkillId))
	{
		return row->Asset;
	}

	return TSoftObjectPtr<UMWSkillAsset>();
}

FPrimaryAssetId UMWSkillDataManager::GetPrimaryAssetIdForSkill(int32 SkillId) const
{
	if (SkillId == INDEX_NONE)
	{
		return FPrimaryAssetId();
	}

	// 先走表校验，避免非法 Id 参与后续加载。
	if (!FindSkillRow(SkillId))
	{
		return FPrimaryAssetId();
	}

	return FPrimaryAssetId(
		FPrimaryAssetType(UMWSkillAsset::PrimaryAssetTypeName),
		FName(*FString::FromInt(SkillId)));
}

UMWSkillAsset* UMWSkillDataManager::GetLoadedSkillAsset(int32 SkillId) const
{
	const FPrimaryAssetId id = GetPrimaryAssetIdForSkill(SkillId);
	if (!id.IsValid())
	{
		return nullptr;
	}

	return Cast<UMWSkillAsset>(UAssetManager::Get().GetPrimaryAssetObject(id));
}

TSharedPtr<FStreamableHandle> UMWSkillDataManager::AsyncLoadSkillBundles(
	int32 SkillId,
	const TArray<FName>& BundlesToLoad,
	FStreamableDelegate OnComplete)
{
	const FPrimaryAssetId id = GetPrimaryAssetIdForSkill(SkillId);
	if (!id.IsValid())
	{
		UE_LOG(LogMWData, Warning, TEXT("AsyncLoadSkillBundles: invalid SkillId %d"), SkillId);
		OnComplete.ExecuteIfBound();
		return nullptr;
	}

	return UAssetManager::Get().ChangeBundleStateForPrimaryAssets(
		{ id },
		BundlesToLoad,
		{},
		false,
		OnComplete,
		FStreamableManager::AsyncLoadHighPriority);
}

UMWSkillAsset* UMWSkillDataManager::SyncLoadSkillBundles(int32 SkillId, const TArray<FName>& BundlesToLoad)
{
	const FPrimaryAssetId id = GetPrimaryAssetIdForSkill(SkillId);
	if (!id.IsValid())
	{
		return nullptr;
	}

	TSharedPtr<FStreamableHandle> handle = UAssetManager::Get().ChangeBundleStateForPrimaryAssets(
		{ id },
		BundlesToLoad,
		{},
		false,
		FStreamableDelegate(),
		FStreamableManager::AsyncLoadHighPriority);

	if (handle.IsValid() && !handle->HasLoadCompleted())
	{
		handle->WaitUntilComplete();
	}

	return Cast<UMWSkillAsset>(UAssetManager::Get().GetPrimaryAssetObject(id));
}

void UMWSkillDataManager::UnloadSkill(int32 SkillId)
{
	const FPrimaryAssetId id = GetPrimaryAssetIdForSkill(SkillId);
	if (id.IsValid())
	{
		UAssetManager::Get().UnloadPrimaryAsset(id);
	}
}

bool UMWSkillDataManager::RebuildSkillTableCache()
{
	SkillRowById.Reset();

	if (!IsValid(SkillTable))
	{
		UE_LOG(LogMWData, Warning, TEXT("RebuildSkillTableCache: SkillTable is null."));
		return false;
	}

	static const FString ContextString(TEXT("UMWSkillDataManager::RebuildSkillTableCache"));
	TArray<FMWSkillTable*> rows;
	SkillTable->GetAllRows(ContextString, rows);

	bool bAllValid = true;

	for (const FMWSkillTable* row : rows)
	{
		if (!row)
		{
			continue;
		}

		if (!row->IsValidRow())
		{
			UE_LOG(LogMWData, Warning, TEXT("SkillTable invalid row: Id=%d Name=%s"), row->Id, *row->Name.ToString());
			bAllValid = false;
			continue;
		}

		if (SkillRowById.Contains(row->Id))
		{
			UE_LOG(LogMWData, Warning, TEXT("SkillTable duplicate Id: %d"), row->Id);
			bAllValid = false;
			continue;
		}

		SkillRowById.Add(row->Id, *row);
	}

	UE_LOG(LogMWData, Log, TEXT("SkillTable cache built. RowCount=%d"), SkillRowById.Num());
	return bAllValid;
}

bool UMWSkillDataManager::LoadSkillTableFromConfig()
{
	const UMWGameSettings* settings = GetDefault<UMWGameSettings>();
	if (!settings || settings->SkillTableAsset.IsNull())
	{
		UE_LOG(LogMWData, Warning, TEXT("SkillDataManager: SkillTableAsset is not configured."));
		SkillTable = nullptr;
		return false;
	}

	SkillTable = settings->SkillTableAsset.LoadSynchronous();
	if (!IsValid(SkillTable))
	{
		UE_LOG(LogMWData, Warning, TEXT("SkillDataManager: failed to load SkillTableAsset: %s"),
			*settings->SkillTableAsset.ToSoftObjectPath().ToString());
		return false;
	}

	return true;
}
