#pragma once

#include "MWDefineGameplay.generated.h"

UENUM(BlueprintType)
enum ECharacterBehaviorState
{
	Normal,
	Battle
};

UENUM()
enum class EBattleResult
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
};

UENUM(BlueprintType)
enum class EMWBattleActionBuff
{
	Idle,
	Attack,
	Heal
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
	/* The speed of party unit, it helps to decide when to move. */
	int32 GetTeamSpeed() const;

	FMWTeamUnit GetLeader() const;

	FMWTeamUnit GetMember(int Index) const;

	FORCEINLINE bool IsAlive() const { return bIsAlive; }

	FORCEINLINE void SetAlive(bool NewValue) {  bIsAlive = NewValue; }

	FORCEINLINE EMWTeamAlign GetTeamAlign() const { return Align; }

	FORCEINLINE bool operator==(const FMWTeam& Other) { return TeamId == Other.TeamId; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TeamId = INDEX_NONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TeamNo = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMWTeamAlign Align = EMWTeamAlign::Player;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FMWTeamUnit> Units;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsAlive = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin=0))
	int32 ActionPoints = 10;
};

FORCEINLINE bool operator==(const FMWTeam& Lhs, const FMWTeam& Rhs) { return Lhs.TeamId == Rhs.TeamId; }

USTRUCT(BlueprintType)
struct FMWBattleSceneParam
{
	GENERATED_BODY()

	UPROPERTY()
	FVector Origin = FVector::ZeroVector;

	UPROPERTY()
	TArray<FMWTeam> Teams;
};

USTRUCT(BlueprintType)
struct FMWBattleData
{
	GENERATED_BODY()

	/* */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bPlayerFirst = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FMWTeam> Teams;
};