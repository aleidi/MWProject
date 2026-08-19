#pragma once

#include "Engine/DataTable.h"
#include "MWCharacterTable.generated.h"

class UMWCharacterAsset;

/**
 * DataTableからロードするキャラクター行データです。
 *
 * 方針：
 * - キャラクターの静的な基本データは本テーブルに保持します。
 * - 詳細なアセットデータはUMWCharacterAsset（PrimaryData）に保持します。
 * - 本行はPrimaryDataアセットへのソフト参照を保持します。
 */
USTRUCT(BlueprintType)
struct FMWCharacterTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	FMWCharacterTable();

	/** キャラクター固有ID（通常はExcelから取得）。 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character")
	int32 Id = INDEX_NONE;

	/** 内部名キー。 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character")
	FName Name = NAME_None;

	/** ローカライズ済み表示名。 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character")
	FText DisplayName = FText::GetEmpty();

	/** キャラクターPrimaryAssetDataへのソフト参照。 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character")
	TSoftObjectPtr<UMWCharacterAsset> Asset;

	/** 行データの最低限の妥当性を確認します。 */
	bool IsValidRow() const;
};