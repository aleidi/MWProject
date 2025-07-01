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
struct FMWCharacterMasterData : public FTableRowBase
{
	GENERATED_BODY()

	/* Id of the character. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 Id;

	/* character blueprint class */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftClassPtr<class AMWCharacter> CharacterClass;

	/* character used animation data */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	TSoftObjectPtr<UMWCharacterAnimData> CharacterAnim;

	/* character ability data */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	TArray<TObjectPtr<class UMWAbilitySet>> AbilitySets;
};