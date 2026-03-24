#pragma once

#include "MWCharacterDataTypes.generated.h"

USTRUCT(BlueprintType)
struct FMWCharacterAppearanceData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AssetBundles = "Base"))
	TSoftObjectPtr<USkeletalMesh> Mesh_Body;
};

USTRUCT(BlueprintType)
struct FMWCharacterAnimData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AssetBundles="Base"))
	TSoftObjectPtr<UAnimSequenceBase> Anim_Idle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AssetBundles="Base"))
	TSoftObjectPtr<UAnimSequenceBase> Anim_Walk;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AssetBundles="Base"))
	TSoftObjectPtr<UAnimSequenceBase> Anim_Run;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AssetBundles="Base"))
	TSoftObjectPtr<UAnimSequenceBase> Anim_Sprint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AssetBundles="Base"))
	TSoftObjectPtr<UAnimSequenceBase> Anim_Jump;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AssetBundles="Base"))
	TSoftObjectPtr<UAnimSequenceBase> Anim_Fall;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AssetBundles="Base"))
	TSoftObjectPtr<UAnimSequenceBase> Anim_Land;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AssetBundles="Base"))
	TSoftObjectPtr<UAnimSequenceBase> Anim_Hit;
};
