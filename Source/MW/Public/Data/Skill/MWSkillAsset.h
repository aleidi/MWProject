#pragma once

#include "Data/MWPrimaryAsset.h"
#include "GameplayTagContainer.h"
#include "MWSkillAsset.generated.h"

class UAnimSequenceBase;
class UTexture2D;

UENUM(BlueprintType)
enum class EMWSkillType : uint8
{
	Active,
	Passive,
	Ultimate,
};

/**
 * Static definition of a skill. One asset per skill design.
 *
 * Naming convention:
 *   PrimaryAssetType : "Skill"
 *   PrimaryAssetName : Id (int32, from Excel)
 *   Asset file name  : DA_Skill_<Id>_<Name>
 */
UCLASS(BlueprintType, HideCategories = ("Asset"))
class MW_API UMWSkillAsset : public UMWPrimaryAsset
{
	GENERATED_BODY()

public:
	/** Canonical PrimaryAssetType for Skill PDAs. Use this everywhere instead of literals. */
	static const FName PrimaryAssetTypeName;

	/** AssetBundle names used by skill loading. */
	static const FName BundleName_Cast;
	static const FName BundleName_UI;

public:
	UMWSkillAsset();

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, AssetRegistrySearchable)
	int32 Id = INDEX_NONE;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AssetBundles = "Cast"))
	TSoftObjectPtr<UAnimMontage> Animation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI", meta = (AssetBundles = "UI"))
	TSoftObjectPtr<UTexture2D> Icon;
};