#include "Data/MWPrimaryAsset.h"

FPrimaryAssetId UMWPrimaryAsset::GetPrimaryAssetId() const
{
	FName primaryAssetName = PrimaryAssetName;

	if (primaryAssetName == NAME_None)
	{
		FString assetName = GetFName().ToString();

		// Asset naming convention: DA_NativeClassNameWithoutPrefixMW_ItemName
		// Extract the ItemName part after the last '_'
		int32 lastUnderscoreIndex;
		if (assetName.FindLastChar(TEXT('_'), lastUnderscoreIndex))
		{
			assetName.RightChopInline(lastUnderscoreIndex + 1);
		}

		primaryAssetName = FName(*assetName);
	}

	return FPrimaryAssetId(PrimaryAssetType, primaryAssetName);
}