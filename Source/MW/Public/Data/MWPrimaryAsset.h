#pragma once

// Include Header
#include "Engine/DataAsset.h"
#include "MWPrimaryAsset.generated.h"

// Forward Declare

// Define

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
	/** PrimaryAssetId =  PrimaryAssetType + FileName.
	*   The name of the derived class asset should follow the convention: 
	*   DA_NativeClassNameWithoutPrefixMW_ItemName, e.g. DA_CharacterData_KOSMOS, DA_JobClassData_Warrior, etc.
	*/
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

protected:
	UPROPERTY()
	FName PrimaryAssetType = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Asset")
	FName PrimaryAssetName = NAME_None;
};