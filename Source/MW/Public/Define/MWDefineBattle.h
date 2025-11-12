#pragma once

#include "MWDefineGameplay.h"
#include "MWDefineBattle.generated.h"

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
	FMWCharacterSkillTable SkillTable;
};