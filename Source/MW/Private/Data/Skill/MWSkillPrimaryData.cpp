#include "Data/Skill/MWSkillPrimaryData.h"

UMWSkillPrimaryData::UMWSkillPrimaryData()
{
	PrimaryAssetType = TEXT("Skill");
}

FPrimaryAssetId UMWSkillPrimaryData::GetPrimaryAssetId() const
{
	if (Id != INDEX_NONE)
	{
		return FPrimaryAssetId(PrimaryAssetType, FName(*FString::FromInt(Id)));
	}

	return Super::GetPrimaryAssetId();
}