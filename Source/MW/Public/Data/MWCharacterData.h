#pragma once

#include "Engine/DataAsset.h"
#include "Define/MWDefineGameplay.h"
#include "MWCharacterData.generated.h"

UCLASS(BlueprintType, Meta = (DisplayName = "MW Character Animation Data", ShortTooltip = "Animation used by pawn."))
class UMWCharacterAnimData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anim")
	FSoftObjectPath Move;
};

UCLASS(BlueprintType)
class UMWCharacterBattleSkillDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	TObjectPtr<UAnimMontage> ApproachAnimation = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	TObjectPtr<UAnimMontage> ReturnAnimation = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FMWCharacterBattleSkillGroup> SkillCombos;

};