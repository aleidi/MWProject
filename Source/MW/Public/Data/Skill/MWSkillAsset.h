#pragma once

#include "Data/MWPrimaryAsset.h"
#include "MWSkillAsset.generated.h"

class UAnimSequenceBase;
class UMWAbilitySet;

UENUM(BlueprintType)
enum class EMWSkillType : uint8
{
	Active,    
	Passive,   
	Ultimate,   
};

/** Skill animation set, loaded via the "Cast" bundle. */
USTRUCT(BlueprintType)
struct FMWSkillAnimSet
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AssetBundles="Cast"))
	TSoftObjectPtr<UAnimSequenceBase> Anim_Cast;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AssetBundles="Cast"))
	TSoftObjectPtr<UAnimSequenceBase> Anim_HitReact;
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

public:
	UMWSkillAsset();

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, AssetRegistrySearchable)
	int32 Id = INDEX_NONE;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName Name = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText DisplayName = FText::GetEmpty();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Description = FText::GetEmpty();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EMWSkillType Type = EMWSkillType::Active;

	/** The animations of this skill. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability")
	FMWSkillAnimSet AnimSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UI")
	TSoftObjectPtr<UTexture2D> Icon;

	/** Max upgrade level. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Progression", meta=(ClampMin="1"))
	int32 MaxLevel = 1;
};