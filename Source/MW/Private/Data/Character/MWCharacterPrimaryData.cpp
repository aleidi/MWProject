#include "Data/Character/MWCharacterPrimaryData.h"

const FName UMWCharacterPrimaryData::PrimaryAssetTypeName = TEXT("Character");
const FName UMWCharacterPrimaryData::BundleName_Spawn     = TEXT("Spawn");
const FName UMWCharacterPrimaryData::BundleName_Extra     = TEXT("Extra");

UMWCharacterPrimaryData::UMWCharacterPrimaryData()
{
	PrimaryAssetType = PrimaryAssetTypeName;
}

FPrimaryAssetId UMWCharacterPrimaryData::GetPrimaryAssetId() const
{
	if (Id != INDEX_NONE)
	{
		return FPrimaryAssetId(PrimaryAssetType, FName(*FString::FromInt(Id)));
	}

	return Super::GetPrimaryAssetId();
}