#pragma once

// Include Header
#include "Interface/MWManagerInterface.h"
#include "Engine/StreamableManager.h"
#include "UObject/NoExportTypes.h"
#include "MWSkillDataManager.generated.h"

// Forward Declare
class UMWSkillAsset;

// Define
#define GET_SKILLDATAMGR(WorldContext)  (UMWSkillDataManager::Get(WorldContext))

/*
 * @class UMWSkillDataManager
 *
 * @brief 通过 PrimaryAsset + AssetBundle 管理技能数据的异步加载。
 */
UCLASS()
class UMWSkillDataManager : public UObject, public IMWManagerInterface
{
	GENERATED_BODY()
	
public:
	UMWSkillDataManager();

	virtual void Initialize() override {}
	virtual void Deinitialize() override {}

	UFUNCTION(BlueprintPure, Category="Manager", meta=(WorldContext="WorldContext", DisplayName="GetSkillDataManager"))
	static UMWSkillDataManager* Get(const UObject* WorldContext);

	UFUNCTION(BlueprintPure, Category="Manager|Skill")
	FPrimaryAssetId GetPrimaryAssetIdForSkill(int32 SkillId) const;

	UFUNCTION(BlueprintPure, Category="Manager|Skill")
	UMWSkillAsset* GetLoadedSkillAsset(int32 SkillId) const;

	TSharedPtr<FStreamableHandle> AsyncLoadSkillBundles(
		int32 SkillId,
		const TArray<FName>& BundlesToLoad,
		FStreamableDelegate OnComplete = FStreamableDelegate());

	UMWSkillAsset* SyncLoadSkillBundles(int32 SkillId, const TArray<FName>& BundlesToLoad);

	UFUNCTION(BlueprintCallable, Category="Manager|Skill")
	void UnloadSkill(int32 SkillId);
};