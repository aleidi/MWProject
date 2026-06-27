#pragma once

#include "CoreMinimal.h"
#include "GameplayAbility/Ability/Skill/MWSkillCastTypes.h"
#include "UObject/Object.h"
#include "MWSkillCastPayload.generated.h"

/** 统一施法载荷（过渡/从零版本通用） */
UCLASS(BlueprintType)
class MW_API UMWSkillCastPayload : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MW|Skill")
	FMWSkillCastCommand Command;
};