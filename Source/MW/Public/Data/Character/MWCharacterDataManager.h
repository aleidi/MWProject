#pragma once

// ヘッダーをインクルード
#include "Data/Character/MWCharacterTable.h"
#include "Interface/MWManagerInterface.h"
#include "Engine/StreamableManager.h"
#include "UObject/NoExportTypes.h"
#include "MWCharacterDataManager.generated.h"

// 前方宣言
class UDataTable;
class UMWCharacterAsset;

// マクロ定義
#define GET_CHARDATAMGR(WorldContext)  (UMWCharacterDataManager::Get(WorldContext))

/*
 * @class UMWCharacterDataManager
 *
 * @brief CharacterTable（軽量インデックス）とPrimaryAsset（リソース本体）を使用してキャラクターデータを管理します。
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

	/** CharacterIdを使用してテーブル行を高速に取得します（キャッシュヒット時はO(1)）。 */
	const FMWCharacterTable* FindCharacterRow(int32 CharacterId) const;

	/** C++およびBlueprintから利用しやすい行コピーインターフェースです。 */
	UFUNCTION(BlueprintPure, Category="Manager|Character")
	bool TryGetCharacterRow(int32 CharacterId, FMWCharacterTable& OutRow) const;

	/** CharacterIdに対応するCharacterAssetのソフト参照を取得します。 */
	UFUNCTION(BlueprintPure, Category="Manager|Character")
	TSoftObjectPtr<UMWCharacterAsset> GetCharacterAssetRef(int32 CharacterId) const;

	/** CharacterIdからPrimaryAssetIdを生成します。 */
	UFUNCTION(BlueprintPure, Category="Manager|Character")
	FPrimaryAssetId GetPrimaryAssetIdForCharacter(int32 CharacterId) const;

	/** メモリにロード済みのPDAを返します。未ロードの場合はnullptrを返します。 */
	UFUNCTION(BlueprintPure, Category="Manager|Character")
	UMWCharacterAsset* GetLoadedCharacterAsset(int32 CharacterId) const;

	/**
	 * 指定したキャラクターのAssetBundleを非同期でロードします。
	 * 必要なBundleがすでにロード済みの場合、OnCompleteは現在または次のフレームで実行されます。
	 */
	TSharedPtr<FStreamableHandle> AsyncLoadCharacterBundles(
		int32 CharacterId,
		const TArray<FName>& BundlesToLoad,
		FStreamableDelegate OnComplete = FStreamableDelegate());

	/** 同期ロードです。ロード画面など、処理の停止が許容される場面でのみ使用します。 */
	UMWCharacterAsset* SyncLoadCharacterBundles(int32 CharacterId, const TArray<FName>& BundlesToLoad);

	/** キャラクターが保持するBundleをアンロードします（キャラクター破棄時などに呼び出します）。 */
	UFUNCTION(BlueprintCallable, Category="Manager|Character")
	void UnloadCharacter(int32 CharacterId);

	/** CharacterTableのキャッシュを再構築します（エディタでのホットリロード時などに使用できます）。 */
	UFUNCTION(BlueprintCallable, Category="Manager|Character")
	bool RebuildCharacterTableCache();

private:
	bool LoadCharacterTableFromConfig();

private:
	/** 設定されたパスからロードしたCharacterTableです。 */
	UPROPERTY(Transient)
	TObjectPtr<UDataTable> CharacterTable = nullptr;

	/** 実行時の高速インデックス：CharacterId -> RowData。 */
	TMap<int32, FMWCharacterTable> CharacterRowById;
};