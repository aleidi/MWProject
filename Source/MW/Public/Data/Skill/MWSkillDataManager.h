#pragma once

// Include Header
#include "Data/Skill/MWSkillTable.h"
#include "Interface/MWManagerInterface.h"
#include "Engine/StreamableManager.h"
#include "UObject/NoExportTypes.h"
#include "MWSkillDataManager.generated.h"

// Forward Declare
class UDataTable;
class UMWSkillAsset;

// Define
#define GET_SKILLDATAMGR(WorldContext)  (UMWSkillDataManager::Get(WorldContext))

/*
 * @class UMWSkillDataManager
 *
 * @brief 通过 SkillTable(轻量索引) + PrimaryAsset(资源实体) 管理技能数据。
 */
UCLASS()
class MW_API UMWSkillDataManager : public UObject, public IMWManagerInterface
{
	GENERATED_BODY()

public:
	virtual void Initialize() override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintPure, Category = "Manager", meta = (WorldContext = "WorldContext", DisplayName = "GetSkillDataManager"))
	static UMWSkillDataManager* Get(const UObject* WorldContext);

	/** 快速查询：按 SkillId 获取表行（缓存命中 O(1)）。 */
	const FMWSkillTable* FindSkillRow(int32 SkillId) const;

	/** C++/Blueprint 友好的行拷贝接口。 */
	UFUNCTION(BlueprintPure, Category = "Manager|Skill")
	bool TryGetSkillRow(int32 SkillId, FMWSkillTable& OutRow) const;

	/** 按 SkillId 获取 SkillAsset 软引用。 */
	UFUNCTION(BlueprintPure, Category = "Manager|Skill")
	TSoftObjectPtr<UMWSkillAsset> GetSkillAssetRef(int32 SkillId) const;

	/** 通过 SkillId 构造 PrimaryAssetId。 */
	UFUNCTION(BlueprintPure, Category = "Manager|Skill")
	FPrimaryAssetId GetPrimaryAssetIdForSkill(int32 SkillId) const;

	/** 返回当前已加载到内存的 PDA；未加载则为 nullptr。 */
	UFUNCTION(BlueprintPure, Category = "Manager|Skill")
	UMWSkillAsset* GetLoadedSkillAsset(int32 SkillId) const;

	/**
	 * 异步加载指定 skill 的若干 AssetBundles。
	 * 如果 bundle 已经满足，OnComplete 会在本帧或下一帧立即触发。
	 */
	TSharedPtr<FStreamableHandle> AsyncLoadSkillBundles(
		int32 SkillId,
		const TArray<FName>& BundlesToLoad,
		FStreamableDelegate OnComplete = FStreamableDelegate());

	/** 阻塞式加载，仅在 loading screen 等可接受卡顿的场景使用。 */
	UMWSkillAsset* SyncLoadSkillBundles(int32 SkillId, const TArray<FName>& BundlesToLoad);

	/** 卸载 skill 持有的 bundle。 */
	UFUNCTION(BlueprintCallable, Category = "Manager|Skill")
	void UnloadSkill(int32 SkillId);

	/** 重新构建 SkillTable 缓存（编辑器热更/重载可调用）。 */
	UFUNCTION(BlueprintCallable, Category = "Manager|Skill")
	bool RebuildSkillTableCache();

private:
	bool LoadSkillTableFromConfig();

private:
	/** Loaded SkillTable from config path. */
	UPROPERTY(Transient)
	TObjectPtr<UDataTable> SkillTable = nullptr;

	/** Runtime fast index: SkillId -> RowData. */
	TMap<int32, FMWSkillTable> SkillRowById;
};