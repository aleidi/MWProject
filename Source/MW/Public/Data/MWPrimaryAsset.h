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
	/** PrimaryAssetId =  PrimaryType + FileName. */
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

protected:

};