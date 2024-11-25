#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GameplayTagContainer.h"
#include "MWSkillSystem.generated.h"

USTRUCT(BlueprintType)
struct FMWSkill
{
	GENERATED_BODY()

	UPROPERTY()
	int32 Id = -1;

	UPROPERTY()
	FName DisplayName;

	UPROPERTY()
	FName Name;

	UPROPERTY()
	FGameplayTag Tag;

	UPROPERTY()
	int32 Cost;

	FORCEINLINE bool IsValid() const { return Id != -1; }
};

USTRUCT(BlueprintType)
struct FMWFinishSkill : public FMWSkill
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 CoolDown;

	/* unrecoverable time of ap */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 PenaltyDuration;
};

USTRUCT(BlueprintType)
struct FMWSkillGroup
{
	GENERATED_BODY()
	
	UPROPERTY()
	TArray<FMWSkill> Skills;

	FMWSkillGroup();
};

/**
 * 
 */
class MW_API TMWSkillSystem : public TSharedFromThis<TMWSkillSystem>
{

public:
	TMWSkillSystem();

	TMWSkillSystem(AActor* InOwner);
	
	/* GroupId - which group of skill to use
	*  SkillId - the index of a skill in a skill group
	*  RealTriggerFunc - the function that actually triggers the skill
	*/
	bool TriggerSkill(int32 GroupId, int32 SkillId, TFunction<bool(const FGameplayTag&)> RealTriggerFunc);

	/* GroupId - which group of skill to use
	*  SkillId - the index of a skill in a skill group
	*/
	bool SetSkill(int32 GroupId, int32 SkillId, const FMWSkill& NewSkill);

private:
	/* Ensure GroupId and SkillId is in the range of setting */
	bool IsSkillIdValid(int32 GroupdId, int32 SkillId) const;

private:
	TArray<FMWSkillGroup> SkillGroups;
	
	TObjectPtr<AActor> Owner;
};
