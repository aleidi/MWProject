#include "Data/Skill/MWSkillDataManager.h"

#include "Data/Skill/MWSkillAsset.h"
#include "Engine/AssetManager.h"
#include "MWLogChannels.h"
#include "System/MWGameInstanceSubsystem.h"

UMWSkillDataManager::UMWSkillDataManager()
{
}

UMWSkillDataManager* UMWSkillDataManager::Get(const UObject* WorldContext)
{
	if (UMWGameInstanceSubsystem* subsystem = UMWGameInstanceSubsystem::Get(WorldContext))
	{
		return subsystem->GetSkillDataManager();
	}

	return nullptr;
}

FPrimaryAssetId UMWSkillDataManager::GetPrimaryAssetIdForSkill(int32 SkillId) const
{
	if (SkillId == INDEX_NONE)
	{
		return FPrimaryAssetId();
	}

	return FPrimaryAssetId(FPrimaryAssetType(UMWSkillAsset::PrimaryAssetTypeName), FName(*FString::FromInt(SkillId)));
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
