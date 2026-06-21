#include "Data/Character/MWCharacterAsset.h"

const FName UMWCharacterAsset::PrimaryAssetTypeName = TEXT("Character");
const FName UMWCharacterAsset::BundleName_Spawn     = TEXT("Spawn");
const FName UMWCharacterAsset::BundleName_Extra     = TEXT("Extra");

UMWCharacterAsset::UMWCharacterAsset()
{
	PrimaryAssetType = PrimaryAssetTypeName;
}

FPrimaryAssetId UMWCharacterAsset::GetPrimaryAssetId() const
{
	if (Id != INDEX_NONE)
	{
		return FPrimaryAssetId(PrimaryAssetType, FName(*FString::FromInt(Id)));
	}

	return Super::GetPrimaryAssetId();
}