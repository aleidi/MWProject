#include "Data/Skill/MWSkillAsset.h"

const FName UMWSkillAsset::PrimaryAssetTypeName = TEXT("Skill");

UMWSkillAsset::UMWSkillAsset()
{
	PrimaryAssetType = PrimaryAssetTypeName;
}

FPrimaryAssetId UMWSkillAsset::GetPrimaryAssetId() const
{
	if (Id != INDEX_NONE)
	{
		return FPrimaryAssetId(PrimaryAssetType, FName(*FString::FromInt(Id)));
	}

	return Super::GetPrimaryAssetId();
}