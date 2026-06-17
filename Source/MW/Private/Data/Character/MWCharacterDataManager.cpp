#include "Data/Character/MWCharacterDataManager.h"
#include "Data/Character/MWCharacterPrimaryData.h"
#include "Engine/AssetManager.h"
#include "MWLogChannels.h"
#include "System/MWGameInstanceSubsystem.h"

UMWCharacterDataManager::UMWCharacterDataManager()
{
}

UMWCharacterDataManager* UMWCharacterDataManager::Get(const UObject* WorldContext)
{
	if (UMWGameInstanceSubsystem* subsystem = UMWGameInstanceSubsystem::Get(WorldContext))
	{
		return subsystem->GetCharacterDataManager();
	}

	return nullptr;
}

FPrimaryAssetId UMWCharacterDataManager::GetPrimaryAssetIdForCharacter(int32 CharacterId) const
{
	if (CharacterId == INDEX_NONE)
	{
		return FPrimaryAssetId();
	}

	return FPrimaryAssetId(FPrimaryAssetType(UMWCharacterPrimaryData::PrimaryAssetTypeName), FName(*FString::FromInt(CharacterId)));
}

UMWCharacterPrimaryData* UMWCharacterDataManager::GetLoadedCharacterPrimaryData(int32 CharacterId) const
{
	const FPrimaryAssetId id = GetPrimaryAssetIdForCharacter(CharacterId);
	if (!id.IsValid())
	{
		return nullptr;
	}

	return Cast<UMWCharacterPrimaryData>(UAssetManager::Get().GetPrimaryAssetObject(id));
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

UMWCharacterPrimaryData* UMWCharacterDataManager::SyncLoadCharacterBundles(int32 CharacterId, const TArray<FName>& BundlesToLoad)
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

	return Cast<UMWCharacterPrimaryData>(UAssetManager::Get().GetPrimaryAssetObject(id));
}

void UMWCharacterDataManager::UnloadCharacter(int32 CharacterId)
{
	const FPrimaryAssetId id = GetPrimaryAssetIdForCharacter(CharacterId);
	if (id.IsValid())
	{
		UAssetManager::Get().UnloadPrimaryAsset(id);
	}
}
