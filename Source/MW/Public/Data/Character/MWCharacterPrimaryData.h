#pragma once

#include "Data/MWPrimaryAsset.h"
#include "MWCharacterPrimaryData.generated.h"

class UAnimSequenceBase;
class USkeletalMesh;
class UMWAbilitySet;
class UMWCharacterAnimInstance;

/** Character appearance (mesh, etc.). Bundle is decided by the outer property. */
USTRUCT(BlueprintType)
struct FMWCharacterAppearance
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<USkeletalMesh> Mesh_Body;
};

/** Character locomotion / reaction animations. Bundle is decided by the outer property. */
USTRUCT(BlueprintType)
struct FMWCharacterAnimSet
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftClassPtr<UMWCharacterAnimInstance> AnimInstance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UAnimSequenceBase> Anim_Idle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UAnimSequenceBase> Anim_Walk;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UAnimSequenceBase> Anim_Run;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UAnimSequenceBase> Anim_Sprint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UAnimSequenceBase> Anim_Jump;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UAnimSequenceBase> Anim_Fall;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UAnimSequenceBase> Anim_Land;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UAnimSequenceBase> Anim_Hit;
};

/**
 * Unified character configuration PDA.
 *
 * Naming convention:
 *   PrimaryAssetType : "Character"  (see PrimaryAssetTypeName below)
 *   PrimaryAssetName : Id (int32, from Excel)
 *   Asset file name  : DA_Character_<Id>_<Name>   e.g. DA_Character_1001_KOSMOS
 *   Asset directory  : /Game/Data/Characters/<Category>/
 *
 * Bundles:
 *   "Spawn" - required to spawn the character (default mesh/anim/ability)
 *   "Extra" - on-demand extras (alternate appearances, ...)
 */
UCLASS(BlueprintType)
class MW_API UMWCharacterPrimaryData : public UMWPrimaryAsset
{
	GENERATED_BODY()

public:
	UMWCharacterPrimaryData();

	/** Canonical PrimaryAssetType for character PDAs. Use this everywhere instead of literals. */
	static const FName PrimaryAssetTypeName;

	/** AssetBundle names used by character loading. */
	static const FName BundleName_Spawn;
	static const FName BundleName_Extra;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, AssetRegistrySearchable)
	int32 Id = INDEX_NONE;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName Name = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText DisplayName = FText::GetEmpty();

	// ==================== Appearance ====================
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Appearance", meta=(AssetBundles="Spawn"))
	FMWCharacterAppearance DefaultAppearance;

	/** Extra appearances keyed by user-defined index, loaded via "Extra" bundle. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Appearance", meta=(AssetBundles="Extra"))
	TMap<int32, FMWCharacterAppearance> ExtraAppearance;

	// ==================== Animation ====================
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animation", meta=(AssetBundles="Spawn"))
	FMWCharacterAnimSet DefaultAnimation;

	// ==================== Ability ====================
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability", meta=(AssetBundles="Spawn"))
	TSoftObjectPtr<UMWAbilitySet> DefaultAbilitySet;
};