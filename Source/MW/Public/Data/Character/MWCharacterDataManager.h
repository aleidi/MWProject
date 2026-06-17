#pragma once

// Include Header
#include "Interface/MWManagerInterface.h"
#include "Engine/StreamableManager.h"
#include "UObject/NoExportTypes.h"
#include "MWCharacterDataManager.generated.h"

// Forward Declare
class UMWCharacterPrimaryData;

// Define
#define GET_CHARDATAMGR(WorldContext)  (UMWCharacterDataManager::Get(WorldContext))

/*
 * @class UMWCharacterDataManager
 *
 * @brief 通过 PrimaryAsset + AssetBundle 管理角色数据的异步加载。
 *
 * @note 旧的 DataTable 路径已废弃，所有角色配置改由 UMWCharacterPrimaryData 提供。
 */
UCLASS()
class UMWCharacterDataManager : public UObject, public IMWManagerInterface
{
	GENERATED_BODY()
	
public:
	UMWCharacterDataManager();

	virtual void Initialize() override {}
	virtual void Deinitialize() override {}

	UFUNCTION(BlueprintPure, Category="Manager", meta=(WorldContext="WorldContext", DisplayName="GetCharacterDataManager"))
	static UMWCharacterDataManager* Get(const UObject* WorldContext);

	/** 通过 CharacterId 构造 PrimaryAssetId。 */
	UFUNCTION(BlueprintPure, Category="Manager|Character")
	FPrimaryAssetId GetPrimaryAssetIdForCharacter(int32 CharacterId) const;

	/** 返回当前已加载到内存的 PDA；未加载则为 nullptr。 */
	UFUNCTION(BlueprintPure, Category="Manager|Character")
	UMWCharacterPrimaryData* GetLoadedCharacterPrimaryData(int32 CharacterId) const;

	/**
	 * 异步加载指定 character 的若干 AssetBundles。
	 * 如果 bundle 已经满足，OnComplete 会在本帧或下一帧立即触发。
	 */
	TSharedPtr<FStreamableHandle> AsyncLoadCharacterBundles(
		int32 CharacterId,
		const TArray<FName>& BundlesToLoad,
		FStreamableDelegate OnComplete = FStreamableDelegate());

	/** 阻塞式加载，仅在 loading screen 等可接受卡顿的场景使用。 */
	UMWCharacterPrimaryData* SyncLoadCharacterBundles(int32 CharacterId, const TArray<FName>& BundlesToLoad);

	/** 卸载 character 持有的 bundle（角色销毁等场景调用）。 */
	UFUNCTION(BlueprintCallable, Category="Manager|Character")
	void UnloadCharacter(int32 CharacterId);
};