#pragma once

#include "MWDefineGameplay.generated.h"

class AMWBattleUnitAvatar;
class AMWCharacter;

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
	TArray<TObjectPtr<AMWBattleUnitAvatar>> BattleUnits;
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
struct FMWCharacterBattleSkillData
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
struct FMWCharacterBattleSkillGroup
{
	GENERATED_BODY()

	static constexpr int32 MaxSkills = (int32)EMWCharacterSkillComboType::Max;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TMap<EMWCharacterSkillComboType, FMWCharacterBattleSkillData> Skills;

public:
	bool SetSkill(EMWCharacterSkillComboType Type, FMWCharacterBattleSkillData* NewData)
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

	const FMWCharacterBattleSkillData* GetSkill(EMWCharacterSkillComboType Type) const
	{
		if (const FMWCharacterBattleSkillData* Found = Skills.Find(Type))
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

	const FMWCharacterBattleSkillData* GetCentralSkill() const { return GetSkill(EMWCharacterSkillComboType::Central); }
	const FMWCharacterBattleSkillData* GetUpDownSkill() const { return GetSkill(EMWCharacterSkillComboType::UpDown); }
	const FMWCharacterBattleSkillData* GetLeftRightSkill() const { return GetSkill(EMWCharacterSkillComboType::LeftRight); }
};

USTRUCT(BlueprintType)
struct FMWCharacterBattleSkillTable
{
	GENERATED_BODY()

	static constexpr int32 MaxCombos = 4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	TObjectPtr<UAnimMontage> ApproachAnimation = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	TObjectPtr<UAnimMontage> ReturnAnimation = nullptr;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<FMWCharacterBattleSkillGroup> SkillCombos;

public:
	bool AddCombo(const FMWCharacterBattleSkillGroup& Group)
	{
		if (SkillCombos.Num() >= MaxCombos)
		{
			return false;
		}

		SkillCombos.Add(Group);

		return true;
	}

	bool SetComboAt(int32 Index, const FMWCharacterBattleSkillGroup& Group)
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

	const FMWCharacterBattleSkillGroup* GetComboAt(int32 Index) const
	{
		if (Index >= 0 && Index < SkillCombos.Num())
		{
			return &SkillCombos[Index];
		}
		return nullptr;
	}

	int32 Num() const { return SkillCombos.Num(); }

	const FMWCharacterBattleSkillGroup* GetSkillCombo1() const { return GetComboAt(0); }
	const FMWCharacterBattleSkillGroup* GetSkillCombo2() const { return GetComboAt(1); }
	const FMWCharacterBattleSkillGroup* GetSkillCombo3() const { return GetComboAt(2); }
	const FMWCharacterBattleSkillGroup* GetSkillCombo4() const { return GetComboAt(3); }
};

USTRUCT(BlueprintType)
struct FMWCharacterSpawnResourceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Id = INDEX_NONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AMWCharacter> CharacterClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMesh> VisualMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAnimInstance> VisualAnimInst = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Height = 0.f;
};