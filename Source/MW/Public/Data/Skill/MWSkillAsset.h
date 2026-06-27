#pragma once

#include "Data/MWPrimaryAsset.h"
#include "GameplayAbility/Ability/Skill/MWSkillCastTypes.h"
#include "MWSkillAsset.generated.h"

class UTexture2D;

UENUM(BlueprintType)
enum class EMWSkillType : uint8
{
	Active,
	Passive,
	Ultimate,
};

UCLASS(BlueprintType, HideCategories = ("Asset"))
class MW_API UMWSkillAsset : public UMWPrimaryAsset
{
	GENERATED_BODY()

public:
	static const FName PrimaryAssetTypeName;
	static const FName BundleName_Cast;
	static const FName BundleName_UI;

public:
	UMWSkillAsset();

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

	UFUNCTION(BlueprintPure, Category = "MW|Skill")
	FName GetMontageSectionByVariant(EMWSkillCastVariant Variant) const;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, AssetRegistrySearchable)
	int32 Id = INDEX_NONE;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AssetBundles = "Cast"))
	TSoftObjectPtr<UAnimMontage> Animation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MW|Animation")
	FName NormalSection = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MW|Animation")
	FName ChargeSection = TEXT("Charge");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MW|Animation")
	FName ReleaseSection = TEXT("Release");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI", meta = (AssetBundles = "UI"))
	TSoftObjectPtr<UTexture2D> Icon;
};