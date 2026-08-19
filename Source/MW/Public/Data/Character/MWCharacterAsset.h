#pragma once

#include "Data/MWPrimaryAsset.h"
#include "MWCharacterAsset.generated.h"

class UAnimSequenceBase;
class USkeletalMesh;
class UMWAbilitySet;
class UMWCharacterAnimInstance;

/** キャラクターの外観（Meshなど）。Bundleは外側のプロパティで指定します。 */
USTRUCT(BlueprintType)
struct FMWCharacterAppearance
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<USkeletalMesh> Mesh_Body;
};

/** キャラクターの移動／リアクションアニメーション。Bundleは外側のプロパティで指定します。 */
USTRUCT(BlueprintType)
struct FMWCharacterAnimSet
{
	GENERATED_BODY()

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
 * キャラクター設定を一元管理するPDAです。
 *
 * 命名規則：
 *   PrimaryAssetType : "Character"（下記のPrimaryAssetTypeNameを参照）
 *   PrimaryAssetName : Id（int32、Excelから取得）
 *   アセットファイル名：DA_Character_<Id>_<Name>（例：DA_Character_1001_KOSMOS）
 *   アセットディレクトリ：/Game/Data/Characters/<Category>/
 *
 * Bundle：
 *   "Spawn" - キャラクター生成に必要（デフォルトのMesh／Anim／Ability）
 *   "Extra" - 必要に応じてロードする追加要素（別外観など）
 */
UCLASS(BlueprintType, HideCategories = ("Asset"))
class MW_API UMWCharacterAsset : public UMWPrimaryAsset
{
	GENERATED_BODY()

public:
	/** キャラクターPDAの標準PrimaryAssetType。リテラルではなく本定数を使用します。 */
	static const FName PrimaryAssetTypeName;

	/** キャラクターのロードに使用するAssetBundle名。 */
	static const FName BundleName_Spawn;
	static const FName BundleName_Extra;

public:
	UMWCharacterAsset();

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, AssetRegistrySearchable)
	int32 Id = INDEX_NONE;

	// ==================== 外観 ====================
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Appearance", meta=(AssetBundles="Spawn"))
	FMWCharacterAppearance DefaultAppearance;

	/** ユーザー定義Indexをキーとし、"Extra" Bundleでロードする追加外観。 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Appearance", meta=(AssetBundles="Extra"))
	TMap<int32, FMWCharacterAppearance> ExtraAppearance;

	// ==================== アニメーション ====================
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animation", meta=(AssetBundles="Spawn"))
	TSoftClassPtr<UMWCharacterAnimInstance> AnimInstance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animation", meta=(AssetBundles="Spawn"))
	FMWCharacterAnimSet DefaultAnimation;

	// ==================== アビリティ ====================
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability", meta=(AssetBundles="Spawn"))
	TSoftObjectPtr<UMWAbilitySet> DefaultAbilitySet;
};