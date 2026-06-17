#pragma once

#include "Data/MWPrimaryAsset.h"
#include "MWSkillPrimaryData.generated.h"

class UAnimSequenceBase;
class UMWAbilitySet;

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
 * Unified skill configuration PDA.
 *
 * PrimaryAssetType: "Skill"
 * PrimaryAssetName: Id
 *
 * Replaces UMWSkillDataBase / UMWSkillAnimData with a single inline asset.
 */
UCLASS(BlueprintType)
class MW_API UMWSkillPrimaryData : public UMWPrimaryAsset
{
	GENERATED_BODY()

public:
	UMWSkillPrimaryData();

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, AssetRegistrySearchable)
	int32 Id = INDEX_NONE;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName Name = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText DisplayName = FText::GetEmpty();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animation")
	FMWSkillAnimSet Animation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability", meta=(AssetBundles="Cast"))
	TSoftObjectPtr<UMWAbilitySet> AbilitySet;
};