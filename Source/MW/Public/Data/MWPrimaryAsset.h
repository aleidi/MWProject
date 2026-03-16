#pragma once

// Include Header
#include "Engine/DataAsset.h"
#include "MWPrimaryAsset.generated.h"

// Forward Declare

// Define
UENUM()
enum class EPrimaryAssetType : uint8
{
	Core,
	Character,
	Quest,
	CutScene,
	Region,
	UI,
	Max
};

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
	/** PrimaryAssetId =  PrimaryType + FileName. */
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PrimaryData")
	EPrimaryAssetType PrimaryType = EPrimaryAssetType::Max;
};