#pragma once

// ヘッダーをインクルード
#include "Engine/DataAsset.h"
#include "MWPrimaryAsset.generated.h"

// 前方宣言

// 定義

/*
 * @class UMWPrimaryAsset
 * 
 * @brief
 *
 * @note
 */
UCLASS()
class UMWPrimaryAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	/** PrimaryAssetId = PrimaryAssetType + FileName。
	*   派生クラスのアセット名は次の規則に従います：
	*   DA_MW接頭辞を除いたNativeClassName_ItemName（例：DA_CharacterData_KOSMOS、DA_JobClassData_Warrior）。
	*/
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

protected:
	UPROPERTY()
	FName PrimaryAssetType = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Asset")
	FName PrimaryAssetName = NAME_None;
};