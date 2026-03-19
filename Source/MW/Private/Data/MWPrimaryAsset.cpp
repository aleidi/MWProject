#include "Data/MWPrimaryAsset.h"

FPrimaryAssetId UMWPrimaryAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(FName(""), GetFName());
}