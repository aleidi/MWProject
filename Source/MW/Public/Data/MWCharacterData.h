#pragma once

#include "Engine/DataAsset.h"
#include "MWCharacterData.generated.h"

UCLASS(BlueprintType, Meta = (DisplayName = "MW Character Animation Data", ShortTooltip = "Animation used by pawn."))
class UMWCharacterAnimData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anim")
	FSoftObjectPath Move;
};

USTRUCT(BlueprintType)
struct FMWCharacterData : public FTableRowBase
{
	GENERATED_BODY()

	/* Id of the character. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 Id = 0;

	/* character blueprint class*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftClassPtr<class AMWCharacter> CharacterClass;

	/* character used animation data */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	TSoftClassPtr<class UMWCharacterAnimInstance> CharacterAnimInst;

	/* character ability data */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	TArray<TObjectPtr<class UMWAbilitySet>> AbilitySets;

	/* Abilities given to character at the beginning of battle. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	TArray<TSoftClassPtr<class UMWGameplayAbility>> BattleAbilities;
};

UCLASS(BlueprintType)
class UMWCharacterMasterData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character")
	TObjectPtr<UDataTable> CharacterData;
};