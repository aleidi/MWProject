#include "Data/Skill/MWSkillAsset.h"

const FName UMWSkillAsset::PrimaryAssetTypeName = TEXT("Skill");
const FName UMWSkillAsset::BundleName_Cast      = TEXT("Cast");
const FName UMWSkillAsset::BundleName_UI        = TEXT("UI");

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

FName UMWSkillAsset::GetMontageSectionByVariant(EMWSkillCastVariant Variant) const
{
	switch (Variant)
	{
	case EMWSkillCastVariant::Charge:
		return ChargeSection;

	case EMWSkillCastVariant::Normal:

	default:
		return NormalSection;
	}
}
