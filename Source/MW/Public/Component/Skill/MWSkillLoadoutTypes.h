#pragma once

#include "MWSkillLoadoutTypes.generated.h"

UENUM(BlueprintType)
enum class EMWSkillLoadoutSlot : uint8
{
	Slot1 = 0	UMETA(DisplayName = "Slot1"),
	Slot2 = 1	UMETA(DisplayName = "Slot2"),
	Slot3 = 2	UMETA(DisplayName = "Slot3"),
	Slot4 = 3	UMETA(DisplayName = "Slot4"),
	Slot5 = 4	UMETA(DisplayName = "Slot5"),
	Slot6 = 5	UMETA(DisplayName = "Slot6"),

	Max			UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FMWSkillSlot
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	EMWSkillLoadoutSlot Slot = EMWSkillLoadoutSlot::Slot1;

	// Used to find skill template from skill data table.
	// スキルデータテーブルからスキルテンプレートを探すために使用される.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	int32 Id = -1;
};

USTRUCT(BlueprintType)
struct FMWCharacterSkillLoadout
{
	GENERATED_BODY()

	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	TArray<FMWSkillSlot> SkillSlots;
};