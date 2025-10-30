#pragma once

#include "MWDefineGameplay.generated.h"

class UMWBattleUnit;

UENUM(BlueprintType)
enum ECharacterBehaviorState : uint8
{
	Normal,
	Battle
};

UENUM()
enum class EBattleResult : uint8
{
	PlayerWin	= 0,
	EnemyWin	= 1,
	Draw		= 2,
	Max			= 3
};

UENUM(BlueprintType)
enum class EMWTeamAlign : uint8
{
	Player,
	Enemy,
	Max
};

UENUM(BlueprintType)
enum class EMWBattleActionBuff : uint8
{
	Idle,
	Attack,
	Heal
};

UENUM(BlueprintType)
enum class EMWCharacterSkillComboType : uint8
{
	Central		= 0,
	UpDown		= 1,
	LeftRight	= 2,
	Max			= 3
};

/* FMWFoundActorInfo
*  It's a data struct used in searching a selectable actor.
*/
USTRUCT(BlueprintType)
struct FMWFoundActorInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString Name = TEXT("");

	UPROPERTY(BlueprintReadWrite)
	bool bLeft = false;

	UPROPERTY(BlueprintReadWrite)
	float Angle = 0.f;

	bool operator==(const FMWFoundActorInfo& Rhs)
	{
		return Name == Rhs.Name;
	}

	void Reset() { Name.Empty(); }

	FORCEINLINE bool IsValid() const { return Name.IsEmpty(); }
};

FORCEINLINE bool operator==(const FMWFoundActorInfo& Lhs, const FMWFoundActorInfo& Rhs)
{
	return Lhs.Name == Rhs.Name;
}

USTRUCT(BlueprintType)
struct FMWTeamUnit 
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ID = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APawn> Pawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsLeader = false;
};

USTRUCT(BlueprintType)
struct FMWTeam
{
	GENERATED_BODY()

public:
	bool IsAlive() const;

	EMWTeamAlign GetTeamAlign() const;

	bool IsValid() const;

	bool operator==(const FMWTeam& Other);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TeamId = INDEX_NONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TeamNo = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMWTeamAlign Align = EMWTeamAlign::Player;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<UMWBattleUnit>> BattleUnits;
};

bool operator==(const FMWTeam& Lhs, const FMWTeam& Rhs);

USTRUCT(BlueprintType)
struct FMWBattleSceneParam
{
	GENERATED_BODY()

	UPROPERTY()
	FVector Origin = FVector::ZeroVector;

	UPROPERTY()
	FMWTeam PlayerTeam;

	UPROPERTY()
	FMWTeam EnemyTeam;
};

USTRUCT(BlueprintType)
struct FMWBattleData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bPlayerFirst = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FMWTeam> Teams;
};

USTRUCT(BlueprintType)
struct FMWCharacterSkillData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	FName SkillId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	TObjectPtr<UAnimMontage> Animation = nullptr;

	// Whether it requires approaching the target before casting.
	// ターゲットに近づいてからキャストする必要があるか。
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	bool bRequiresApproach = false;

	// The range within which the skill can be executed.
	// スキルを実行できる範囲。
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (EditCondition = "bRequiresApproach"))
	float CastRange = 100.f;

	// Whether to use the special approach animation when approaching the target.
	// ターゲットに接近する際に特別なアプローチアニメーションを使用するか。
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (EditCondition = "bRequiresApproach"))
	bool bUseApproachAnimation = false;
};

/*
 *            Skill Table Diagram
 *
 *        　〇   ーーー   〇   ーーー   〇  ーーー   〇
 *                 |           |           |
 *		  ↑↓＋〇 ーーー ↑↓＋〇 ーーー ↑↓＋〇 ーーー ↑↓＋〇
 * 　　　　         |           |           |
 * 　　　　←→＋〇 ーーー ←→＋〇 ーーー ←→＋〇 ーーー ←→＋〇
 */
USTRUCT(BlueprintType)
struct MW_API FMWCharacterSkillGroup
{
	GENERATED_BODY()

	static constexpr int32 MaxSkills = (int32)EMWCharacterSkillComboType::Max;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TMap<EMWCharacterSkillComboType, FMWCharacterSkillData> Skills;

public:
	bool SetSkill(EMWCharacterSkillComboType Type, FMWCharacterSkillData* NewData)
	{
		if (!NewData)
		{
			Skills.Remove(Type);

			return true;
		}

		if (Skills.Contains(Type))
		{
			Skills[Type] = *NewData;

			return true;
		}
		else if (Skills.Num() < MaxSkills)
		{
			Skills.Emplace(Type, *NewData);

			return true;
		}

		return false;
	}

	const FMWCharacterSkillData* GetSkill(EMWCharacterSkillComboType Type) const
	{
		if (const FMWCharacterSkillData* Found = Skills.Find(Type))
		{
			return Found;
		}

		return nullptr;
	}

	void RemoveSkill(EMWCharacterSkillComboType Type)
	{
		Skills.Remove(Type);
	}

	void Reset()
	{
		Skills.Reset();
	}

	int32 Num() const { return Skills.Num(); }

	const FMWCharacterSkillData* GetCentralSkill() const { return GetSkill(EMWCharacterSkillComboType::Central); }
	const FMWCharacterSkillData* GetUpDownSkill() const { return GetSkill(EMWCharacterSkillComboType::UpDown); }
	const FMWCharacterSkillData* GetLeftRightSkill() const { return GetSkill(EMWCharacterSkillComboType::LeftRight); }
};

USTRUCT(BlueprintType)
struct MW_API FMWCharacterSkillTable
{
	GENERATED_BODY()

	static constexpr int32 MaxCombos = 4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	TObjectPtr<UAnimMontage> ApproachAnimation = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	TObjectPtr<UAnimMontage> ReturnAnimation = nullptr;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<FMWCharacterSkillGroup> SkillCombos;

public:
	bool AddCombo(const FMWCharacterSkillGroup& Group)
	{
		if (SkillCombos.Num() >= MaxCombos)
		{
			return false;
		}

		SkillCombos.Add(Group);

		return true;
	}

	bool SetComboAt(int32 Index, const FMWCharacterSkillGroup& Group)
	{
		if (Index < 0 || Index >= MaxCombos)
		{
			return false;
		}

		if (Index < SkillCombos.Num())
		{
			SkillCombos[Index] = Group;
		}
		else
		{
			SkillCombos.SetNum(Index + 1);
			SkillCombos[Index] = Group;
		}

		return true;
	}

	const FMWCharacterSkillGroup* GetComboAt(int32 Index) const
	{
		if (Index >= 0 && Index < SkillCombos.Num())
		{
			return &SkillCombos[Index];
		}
		return nullptr;
	}

	int32 Num() const { return SkillCombos.Num(); }

	const FMWCharacterSkillGroup* GetSkillCombo1() const { return GetComboAt(0); }
	const FMWCharacterSkillGroup* GetSkillCombo2() const { return GetComboAt(1); }
	const FMWCharacterSkillGroup* GetSkillCombo3() const { return GetComboAt(2); }
	const FMWCharacterSkillGroup* GetSkillCombo4() const { return GetComboAt(3); }
};