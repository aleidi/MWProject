#pragma once

// Include Header
#include "Data/Character/MWCharacterTable.h"
#include "Interface/MWManagerInterface.h"
#include "Engine/StreamableManager.h"
#include "UObject/NoExportTypes.h"
#include "MWCharacterDataManager.generated.h"

// Forward Declare
class UDataTable;
class UMWCharacterAsset;

// Define
#define GET_CHARDATAMGR(WorldContext)  (UMWCharacterDataManager::Get(WorldContext))

/*
 * @class UMWCharacterDataManager
 *
 * @brief 通过 CharacterTable(轻量索引) + PrimaryAsset(资源实体) 管理角色数据。
 */
UCLASS()
class UMWCharacterDataManager : public UObject, public IMWManagerInterface
{
	GENERATED_BODY()
	
public:
	virtual void Initialize() override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintPure, Category="Manager", meta=(WorldContext="WorldContext", DisplayName="GetCharacterDataManager"))
	static UMWCharacterDataManager* Get(const UObject* WorldContext);

	/** 快速查询：按 CharacterId 获取表行（缓存命中 O(1)）。 */
	const FMWCharacterTable* FindCharacterRow(int32 CharacterId) const;

	/** C++/Blueprint 友好的行拷贝接口。 */
	UFUNCTION(BlueprintPure, Category="Manager|Character")
	bool TryGetCharacterRow(int32 CharacterId, FMWCharacterTable& OutRow) const;

	/** 按 CharacterId 获取 CharacterAsset 软引用。 */
	UFUNCTION(BlueprintPure, Category="Manager|Character")
	TSoftObjectPtr<UMWCharacterAsset> GetCharacterAssetRef(int32 CharacterId) const;

	/** 通过 CharacterId 构造 PrimaryAssetId。 */
	UFUNCTION(BlueprintPure, Category="Manager|Character")
	FPrimaryAssetId GetPrimaryAssetIdForCharacter(int32 CharacterId) const;

	/** 返回当前已加载到内存的 PDA；未加载则为 nullptr。 */
	UFUNCTION(BlueprintPure, Category="Manager|Character")
	UMWCharacterAsset* GetLoadedCharacterAsset(int32 CharacterId) const;

	/**
	 * 异步加载指定 character 的若干 AssetBundles。
	 * 如果 bundle 已经满足，OnComplete 会在本帧或下一帧立即触发。
	 */
	TSharedPtr<FStreamableHandle> AsyncLoadCharacterBundles(
		int32 CharacterId,
		const TArray<FName>& BundlesToLoad,
		FStreamableDelegate OnComplete = FStreamableDelegate());

	/** 阻塞式加载，仅在 loading screen 等可接受卡顿的场景使用。 */
	UMWCharacterAsset* SyncLoadCharacterBundles(int32 CharacterId, const TArray<FName>& BundlesToLoad);

	/** 卸载 character 持有的 bundle（角色销毁等场景调用）。 */
	UFUNCTION(BlueprintCallable, Category="Manager|Character")
	void UnloadCharacter(int32 CharacterId);

	/** 重新构建 CharacterTable 缓存（编辑器热更/重载可调用）。 */
	UFUNCTION(BlueprintCallable, Category="Manager|Character")
	bool RebuildCharacterTableCache();

private:
	bool LoadCharacterTableFromConfig();

private:
	/** Loaded CharacterTable from config path. */
	UPROPERTY(Transient)
	TObjectPtr<UDataTable> CharacterTable = nullptr;

	/** Runtime fast index: CharacterId -> RowData. */
	TMap<int32, FMWCharacterTable> CharacterRowById;
};