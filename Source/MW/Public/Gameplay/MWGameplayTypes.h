#pragma once

#include "MWGameplayTypes.generated.h"

#define DT_CHARACTERINFO TEXT("/Game/Datatable/Character/DT_CharacterInfo.DT_CharacterInfo")

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

#pragma region DataTable
USTRUCT(BlueprintType)
struct FMWCharacterInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Head;
};
#pragma endregion

#pragma region Battle

UENUM()
enum class EBattleResult
{
	PlayerWin	= 0,
	EnemyWin	= 1,
	Draw		= 2,
	Max			= 3
};

UENUM(BlueprintType)
enum class EMWTeamType
{
	Player,
	Enemy,
	Friendly,
	Neutral
};

USTRUCT(BlueprintType)
struct FMWTeamUnit 
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APawn> Pawn;
};

USTRUCT(BlueprintType)
struct FMWTeam
{
	GENERATED_BODY()

public:
	/* The speed of party unit, it helps to decide when to move. */
	int32 GetTeamSpeed() const;

	FMWTeamUnit GetLeader() const;

	FMWTeamUnit GetMember(int Index) const;

	FORCEINLINE bool IsAlive() const { return bIsAlive; }

	FORCEINLINE void SetAlive(bool NewValue) {  bIsAlive = NewValue; }

	FORCEINLINE EMWTeamType GetTeamType() const { return Type; }

	FORCEINLINE bool operator==(const FMWTeam& Other) { return TeamId == Other.TeamId; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TeamId = INDEX_NONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMWTeamType Type = EMWTeamType::Player;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FMWTeamUnit> Units;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxNum = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 LeaderIdx = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsAlive = true;
};

FORCEINLINE bool operator==(const FMWTeam& Lhs, const FMWTeam& Rhs) { return Lhs.TeamId == Rhs.TeamId; }

UENUM(BlueprintType)
enum class EMWBattleActionBuff
{
	Idle,
	Attack,
	Heal
};
#pragma endregion