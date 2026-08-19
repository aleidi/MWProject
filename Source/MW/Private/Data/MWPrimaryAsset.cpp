#include "Data/MWPrimaryAsset.h"

FPrimaryAssetId UMWPrimaryAsset::GetPrimaryAssetId() const
{
	FName primaryAssetName = PrimaryAssetName;

	if (primaryAssetName == NAME_None)
	{
		FString assetName = GetFName().ToString();

		// アセット命名規則: DA_NativeClassNameWithoutPrefixMW_ItemName
		// 最後の「_」以降をItemNameとして抽出
		int32 lastUnderscoreIndex;
		if (assetName.FindLastChar(TEXT('_'), lastUnderscoreIndex))
		{
			assetName.RightChopInline(lastUnderscoreIndex + 1);
		}

		primaryAssetName = FName(*assetName);
	}

	return FPrimaryAssetId(PrimaryAssetType, primaryAssetName);
}