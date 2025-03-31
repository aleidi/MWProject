#pragma once

#include "GameplayAbility/MWGameplayAbility.h"
#include "UObject/NoExportTypes.h"
#include "MWSkillSystem.generated.h"

USTRUCT(BlueprintType)
struct MW_API FSkillGroupNo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 GroupNo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SkillNo;

	FString ToString() const { return FString::Printf(TEXT("[%d, %d]"), GroupNo, SkillNo); }

	bool operator==(const FSkillGroupNo& Rhs)
	{
		return GroupNo == Rhs.GroupNo && SkillNo == Rhs.SkillNo;
	}

	friend inline bool operator==(const FSkillGroupNo& Lhs, const FSkillGroupNo& Rhs)
	{
		return Lhs.GroupNo == Rhs.GroupNo && Lhs.SkillNo == Rhs.SkillNo;
	}

	friend inline uint32 GetTypeHash(const FSkillGroupNo& In)
	{
		return HashCombine(GetTypeHash(In.GroupNo), GetTypeHash(In.SkillNo));
	}
};

USTRUCT(BlueprintType)
struct MW_API FSkillSpec
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UMWSkillBase> SkillAbility;

	FString ToString() const { return FString::Printf(TEXT("%d,%s"), ID, *DisplayName.ToString()); }

	bool operator==(const FSkillSpec& Rhs)
	{
		return ID == Rhs.ID;
	}

	bool operator!=(const FSkillSpec& Rhs)
	{
		return ID != Rhs.ID;
	}

	friend inline bool operator==(const FSkillSpec& Lhs, const FSkillSpec& Rhs)
	{
		return Lhs.ID == Rhs.ID;
	}
};

/* UMWSkillSystem
* 
*  This ability is responsible for the skill management. 
*  It should be instanced by per actor.
*  Only one instance is allowed.
*/
UCLASS()
class MW_API UMWSkillSystem : public UMWGameplayAbility
{
	GENERATED_BODY()

public:
	UMWSkillSystem(const FObjectInitializer& ObjectInitializer);

protected:
	/* This class will never be activated. */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

public:
	/* The skill that is achieved. */
	void GrantSkill(const FSkillSpec& SkillSpec);
	const TArray<FSkillSpec>& GetGrantedSkill() const;
	
	/* Pair the skill and skill group. Skill should be within OwnedSkill. */
	void EquipSkill(const FSkillGroupNo& No, const FSkillSpec& NewSkillSpec);
	const TMap<FSkillGroupNo, FSkillSpec>& GetSetSkill() const;

	/* After the skill is equipped, cast skill by group number. */
	void CastSkill(const FSkillGroupNo& No);
	/* This is used for skill that is not equipped but the skill should be owned. */
	void CastSkill(const FSkillSpec& Skill);

	UFUNCTION(BlueprintCallable, Category = "Ability|Skill")
	void CastSkill(const FSkillGroupNo& No, const FSkillSpec& Skill, bool bForce = true);

private:
	UPROPERTY(EditAnywhere)
	TMap<FSkillGroupNo, FSkillSpec> EquippedSkill;

	UPROPERTY(EditAnywhere)
	TArray<FSkillSpec> OwnedSkill;
};