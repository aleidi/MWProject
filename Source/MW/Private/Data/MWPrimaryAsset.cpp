#include "Data/MWPrimaryAsset.h"

FPrimaryAssetId UMWPrimaryAsset::GetPrimaryAssetId() const
{
	const FString typeName = StaticEnum<EPrimaryAssetType>()->GetNameStringByValue(static_cast<int64>(PrimaryType));
	return FPrimaryAssetId(FName(*typeName), GetFName());
}