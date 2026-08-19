#pragma once

#include "GameplayTagContainer.h"
#include "MWSkillCastTypes.generated.h"

UENUM(BlueprintType)
enum class EMWSkillCastSource : uint8
{
	PlayerInput,
	AI,
	Script,
};

UENUM(BlueprintType)
enum class EMWSkillCastVariant : uint8
{
	Normal,
	Charge,
};

USTRUCT(BlueprintType)
struct FMWSkillCastCommand
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MW|Skill")
	EMWSkillCastSource Source = EMWSkillCastSource::PlayerInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MW|Skill")
	EMWSkillCastVariant Variant = EMWSkillCastVariant::Normal;

	/** 共通実行ではSkillIdを優先して使用します。 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MW|Skill")
	int32 SkillId = INDEX_NONE;

	/** プレイヤー入力経路との互換性を維持するフィールドです。 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MW|Skill")
	int32 SkillSlot = INDEX_NONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MW|Skill")
	FGameplayTag InputTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MW|Skill")
	FName OverrideSection = NAME_None;
};
