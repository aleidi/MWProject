#include "Data/Character/MWCharacterDataManager.h"

#include "Data/Character/MWCharacterAsset.h"
#include "Engine/AssetManager.h"
#include "Engine/DataTable.h"
#include "MWLogChannels.h"
#include "Settings/MWGameSettings.h"
#include "System/MWGameInstanceSubsystem.h"

void UMWCharacterDataManager::Initialize()
{
	CharacterRowById.Reset();

	if (!LoadCharacterTableFromConfig())
	{
		return;
	}

	RebuildCharacterTableCache();
}

void UMWCharacterDataManager::Deinitialize()
{
	CharacterRowById.Reset();
	CharacterTable = nullptr;
}

UMWCharacterDataManager* UMWCharacterDataManager::Get(const UObject* WorldContext)
{
	if (UMWGameInstanceSubsystem* subsystem = UMWGameInstanceSubsystem::Get(WorldContext))
	{
		return subsystem->GetCharacterDataManager();
	}

	return nullptr;
}

const FMWCharacterTable* UMWCharacterDataManager::FindCharacterRow(int32 CharacterId) const
{
	return CharacterRowById.Find(CharacterId);
}

bool UMWCharacterDataManager::TryGetCharacterRow(int32 CharacterId, FMWCharacterTable& OutRow) const
{
	if (const FMWCharacterTable* row = FindCharacterRow(CharacterId))
	{
		OutRow = *row;
		return true;
	}

	return false;
}

TSoftObjectPtr<UMWCharacterAsset> UMWCharacterDataManager::GetCharacterAssetRef(int32 CharacterId) const
{
	if (const FMWCharacterTable* row = FindCharacterRow(CharacterId))
	{
		return row->Asset;
	}

	return TSoftObjectPtr<UMWCharacterAsset>();
}

FPrimaryAssetId UMWCharacterDataManager::GetPrimaryAssetIdForCharacter(int32 CharacterId) const
{
	if (CharacterId == INDEX_NONE)
	{
		return FPrimaryAssetId();
	}

	// 先走表校验，避免非法 Id 参与后续加载。
	if (!FindCharacterRow(CharacterId))
	{
		return FPrimaryAssetId();
	}

	return FPrimaryAssetId(
		FPrimaryAssetType(UMWCharacterAsset::PrimaryAssetTypeName),
		FName(*FString::FromInt(CharacterId)));
}

UMWCharacterAsset* UMWCharacterDataManager::GetLoadedCharacterAsset(int32 CharacterId) const
{
	const FPrimaryAssetId id = GetPrimaryAssetIdForCharacter(CharacterId);
	if (!id.IsValid())
	{
		return nullptr;
	}

	return Cast<UMWCharacterAsset>(UAssetManager::Get().GetPrimaryAssetObject(id));
}

TSharedPtr<FStreamableHandle> UMWCharacterDataManager::AsyncLoadCharacterBundles(
	int32 CharacterId,
	const TArray<FName>& BundlesToLoad,
	FStreamableDelegate OnComplete)
{
	const FPrimaryAssetId id = GetPrimaryAssetIdForCharacter(CharacterId);
	if (!id.IsValid())
	{
		UE_LOG(LogMWData, Warning, TEXT("AsyncLoadCharacterBundles: invalid CharacterId %d"), CharacterId);
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

UMWCharacterAsset* UMWCharacterDataManager::SyncLoadCharacterBundles(int32 CharacterId, const TArray<FName>& BundlesToLoad)
{
	const FPrimaryAssetId id = GetPrimaryAssetIdForCharacter(CharacterId);
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

	return Cast<UMWCharacterAsset>(UAssetManager::Get().GetPrimaryAssetObject(id));
}

void UMWCharacterDataManager::UnloadCharacter(int32 CharacterId)
{
	const FPrimaryAssetId id = GetPrimaryAssetIdForCharacter(CharacterId);
	if (id.IsValid())
	{
		UAssetManager::Get().UnloadPrimaryAsset(id);
	}
}

bool UMWCharacterDataManager::RebuildCharacterTableCache()
{
	CharacterRowById.Reset();

	if (!IsValid(CharacterTable))
	{
		UE_LOG(LogMWData, Warning, TEXT("RebuildCharacterTableCache: CharacterTable is null."));
		return false;
	}

	static const FString ContextString(TEXT("UMWCharacterDataManager::RebuildCharacterTableCache"));
	TArray<FMWCharacterTable*> rows;
	CharacterTable->GetAllRows(ContextString, rows);

	bool bAllValid = true;

	for (const FMWCharacterTable* row : rows)
	{
		if (!row)
		{
			continue;
		}

		if (!row->IsValidRow())
		{
			UE_LOG(LogMWData, Warning, TEXT("CharacterTable invalid row: Id=%d Name=%s"), row->Id, *row->Name.ToString());
			bAllValid = false;
			continue;
		}

		if (CharacterRowById.Contains(row->Id))
		{
			UE_LOG(LogMWData, Warning, TEXT("CharacterTable duplicate Id: %d"), row->Id);
			bAllValid = false;
			continue;
		}

		CharacterRowById.Add(row->Id, *row);
	}

	UE_LOG(LogMWData, Log, TEXT("CharacterTable cache built. RowCount=%d"), CharacterRowById.Num());
	return bAllValid;
}

bool UMWCharacterDataManager::LoadCharacterTableFromConfig()
{
	const UMWGameSettings* settings = GetDefault<UMWGameSettings>();
	if (!settings || settings->CharacterTableAsset.IsNull())
	{
		UE_LOG(LogMWData, Warning, TEXT("CharacterDataManager: CharacterTableAsset is not configured."));
		CharacterTable = nullptr;
		return false;
	}

	CharacterTable = settings->CharacterTableAsset.LoadSynchronous();
	if (!IsValid(CharacterTable))
	{
		UE_LOG(LogMWData, Warning, TEXT("CharacterDataManager: failed to load CharacterTableAsset: %s"),
			*settings->CharacterTableAsset.ToSoftObjectPath().ToString());
		return false;
	}

	return true;
}
