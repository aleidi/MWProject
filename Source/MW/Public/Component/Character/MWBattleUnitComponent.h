#pragma once

// Include Header
#include "Component/Pawn/MWPawnComponent.h"
#include "MWBattleUnitComponent.generated.h"

// Forward Declare
class AMWBattleUnitAvatar;
class UMWCharacterBattleSkillDataAsset;

namespace MWBattle
{
	class FMWStatusEffectManager;
	class FMWAttributeManager;
}


UENUM(BlueprintType)
enum class EBattleUnitCharacterType : uint8
{
	Entity,
	Possession
};

USTRUCT(BlueprintType)
struct FMWBattleUnitCharacterData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ID = INDEX_NONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EBattleUnitCharacterType Type = EBattleUnitCharacterType::Entity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMesh> Mesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAnimInstance> AnimInst = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<const UMWCharacterBattleSkillDataAsset> SkillTable;
};

// Define

/*
 * @class UMWBattleUnitComponent
 * 
 * @brief This component manages the battle unit's avatar.
 *
 * @note
 */
UCLASS(BlueprintType, meta=(BlueprintSpawnableComponent))
class UMWBattleUnitComponent : public UMWPawnComponent
{
	GENERATED_BODY()
	
public:
	UMWBattleUnitComponent(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "MW|Battle|Unit")
	void SetCharacterData(const TArray<FMWBattleUnitCharacterData>& InData);

	/**
	 * Change the avatar's mesh and animation instance.
	 * アバターのメッシュとアニメーションインスタンスを変更する.
	 *
	 * @param  bNext : If true, change to the next character; if false, change to the previous character.
	 *                 Trueの場合は次のキャラクターに変更し、Falseの場合は前のキャラクターに変更する。
	 */
	UFUNCTION(BlueprintCallable, Category = "MW|Battle|Unit")
	void ChangeNextAvatar(bool bNext);

	const UMWCharacterBattleSkillDataAsset* GetSkillData();

private:
	// Attribute manager handles the attributes of this battle unit.
	// アトリビュートマネージャーはこの戦闘ユニットの属性を管理する.
	TSharedPtr<MWBattle::FMWAttributeManager> AttributeManager = nullptr;

	// Status effect manager handles the status effects of this battle unit.
	// ステータスエフェクトマネージャーはこの戦闘ユニットのステータスエフェクトを管理する.
	TSharedPtr<MWBattle::FMWStatusEffectManager> StatusEffectManager = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
	TArray<FMWBattleUnitCharacterData> CharacterData;

	UPROPERTY()
	int32 AvatarCharacterIdx = INDEX_NONE;

};