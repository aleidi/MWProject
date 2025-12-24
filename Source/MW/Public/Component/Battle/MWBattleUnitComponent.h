#pragma once

// Include Header
#include "Component/Pawn/MWPawnComponent.h"
#include "Gameplay/Battle/BattleUnit/MWStatusEffect.h"
#include "MWBattleUnitComponent.generated.h"

// Forward Declare
class AMWCharacter;
class UMWAbilitySet;
class UMWBattleAttributeSet;
class UMWCharacterBattleSkillDataAsset;
struct FMWAbilitySetGrantedHandles;


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

	void OnCharacterDataChanged();

	const UMWCharacterBattleSkillDataAsset* GetSkillData();

	// Activate or deactivate combat state.
	UFUNCTION(BlueprintCallable, Category = "MW|Battle|Unit")
	void SetCombatState(bool bInCombat);

	// Return if battle unit is in combat state.
	bool GetCombatState() const;

protected:
	// Called when entering combat state.
	// 戦闘状態に入るときに呼び出される.
	virtual void OnCombatBegin();

	// Called when exiting combat state.
	// 戦闘状態から出るときに呼び出される.
	virtual void OnCombatEnd();

	void GrandCombatAbilities();

	void RemoveCombatAbilities();

	int32 FindEntityCharacterId() const;

private:
	// Status effect manager handles the status effects of this battle unit.
	// ステータスエフェクトマネージャーはこの戦闘ユニットのステータスエフェクトを管理する.
	TUniquePtr<MWCommandBattle::FMWStatusEffectManager> StatusEffectManager = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
	TArray<FMWBattleUnitCharacterData> CharacterData;

	int32 EntityCharacterIdx = INDEX_NONE;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MW | Ability", meta=(AllowPrivateAccess))
	TObjectPtr<UMWAbilitySet> AbilitySet;

	/* Cache the granted abilities */
	TSharedPtr<FMWAbilitySetGrantedHandles> AbilityGranetedHandles;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MW|Battle", meta=(AllowPrivateAccess))
	bool bIsInCombat = false;

public:
	void BeginPlay() override;
};