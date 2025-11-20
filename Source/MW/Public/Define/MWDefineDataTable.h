#pragma once

#include "Engine/DataAsset.h"
#include "Data/MWCharacterData.h"
#include "MWDefineDataTable.generated.h"

USTRUCT(BlueprintType)
struct FMWCharacterData : public FTableRowBase
{
	GENERATED_BODY()

	/* Id of the character. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 Id = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName Name;

	/* character blueprint class*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Appearance")
	TSoftObjectPtr<USkeletalMesh> CharacterMesh;

	/* character blueprint class*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Appearance")
	float Height = 0.f;

	/* character used animation data */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	TSoftClassPtr<UAnimInstance> CharacterAnimInst;

	/* character ability data */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	TArray<TObjectPtr<class UMWAbilitySet>> BaseAbilitySets;

	/* Abilities given to character at the beginning of battle. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Battle|Ability")
	TArray<TSoftClassPtr<class UMWAbilitySet>> BattleAbilitySets;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Battle|Ability")
	TSoftObjectPtr<UMWCharacterBattleSkillDataAsset> BattleSkillTable;
};
