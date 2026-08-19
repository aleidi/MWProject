#pragma once

// ヘッダーをインクルード
#include "Data/Skill/MWSkillTable.h"
#include "Interface/MWManagerInterface.h"
#include "Engine/StreamableManager.h"
#include "UObject/NoExportTypes.h"
#include "MWSkillDataManager.generated.h"

// 前方宣言
class UDataTable;
class UMWSkillAsset;

// マクロ定義
#define GET_SKILLDATAMGR(WorldContext)  (UMWSkillDataManager::Get(WorldContext))

/*
 * @class UMWSkillDataManager
 *
 * @brief SkillTable（軽量インデックス）とPrimaryAsset（リソース本体）を使用してスキルデータを管理します。
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

	/** SkillIdを使用してテーブル行を高速に取得します（キャッシュヒット時はO(1)）。 */
	const FMWSkillTable* FindSkillRow(int32 SkillId) const;

	/** C++およびBlueprintから利用しやすい行コピーインターフェースです。 */
	UFUNCTION(BlueprintPure, Category = "Manager|Skill")
	bool TryGetSkillRow(int32 SkillId, FMWSkillTable& OutRow) const;

	/** SkillIdに対応するSkillAssetのソフト参照を取得します。 */
	UFUNCTION(BlueprintPure, Category = "Manager|Skill")
	TSoftObjectPtr<UMWSkillAsset> GetSkillAssetRef(int32 SkillId) const;

	/** SkillIdからPrimaryAssetIdを生成します。 */
	UFUNCTION(BlueprintPure, Category = "Manager|Skill")
	FPrimaryAssetId GetPrimaryAssetIdForSkill(int32 SkillId) const;

	/** メモリにロード済みのPDAを返します。未ロードの場合はnullptrを返します。 */
	UFUNCTION(BlueprintPure, Category = "Manager|Skill")
	UMWSkillAsset* GetLoadedSkillAsset(int32 SkillId) const;

	/**
	 * 指定したスキルのAssetBundleを非同期でロードします。
	 * 必要なBundleがすでにロード済みの場合、OnCompleteは現在または次のフレームで実行されます。
	 */
	TSharedPtr<FStreamableHandle> AsyncLoadSkillBundles(
		int32 SkillId,
		const TArray<FName>& BundlesToLoad,
		FStreamableDelegate OnComplete = FStreamableDelegate());

	/** 同期ロードです。ロード画面など、処理の停止が許容される場面でのみ使用します。 */
	UMWSkillAsset* SyncLoadSkillBundles(int32 SkillId, const TArray<FName>& BundlesToLoad);

	/** スキルが保持するBundleをアンロードします。 */
	UFUNCTION(BlueprintCallable, Category = "Manager|Skill")
	void UnloadSkill(int32 SkillId);

	/** SkillTableのキャッシュを再構築します（エディタでのホットリロード時などに使用できます）。 */
	UFUNCTION(BlueprintCallable, Category = "Manager|Skill")
	bool RebuildSkillTableCache();

private:
	bool LoadSkillTableFromConfig();

private:
	/** 設定されたパスからロードしたSkillTableです。 */
	UPROPERTY(Transient)
	TObjectPtr<UDataTable> SkillTable = nullptr;

	/** 実行時の高速インデックス：SkillId -> RowData。 */
	TMap<int32, FMWSkillTable> SkillRowById;
};