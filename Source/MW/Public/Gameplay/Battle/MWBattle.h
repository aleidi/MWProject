#pragma once

#include "CoreMinimal.h"
#include "Gameplay/MWGameplayTypes.h"
#include "MWLogChannels.h"
#include "Gameplay/Battle/MWBattleSystem.h"
#include "MWBattle.generated.h"


// read from data table
#define WAIT_TIME_DEFAULT 100

namespace MWBattle
{
	struct FMWBattleUnit
	{	
		FMWBattleUnit() = default;

		FMWBattleUnit(const FMWBattleUnit& InUnit):Team(InUnit.Team), WaitTime(InUnit.WaitTime)
		{}

		FMWBattleUnit(const FMWTeam& InTeam) : Team(InTeam), WaitTime(WAIT_TIME_DEFAULT)
		{}

		FMWTeam Team;

		int32 WaitTime;

		bool operator ==(const FMWBattleUnit& Other)
		{
			return Team == Other.Team;
		}

		FMWBattleUnit& operator =(const FMWBattleUnit& Other)
		{
			if (this != &Other)
			{
				Team = Other.Team;
				WaitTime = Other.WaitTime;
			}
			return *this;
		}
	};

	FORCEINLINE bool operator ==(const FMWBattleUnit& Lhs, const FMWBattleUnit& Rhs) { return Lhs.Team == Rhs.Team; }

	class IBattleState
	{
	protected:
		typedef UMWBattle BattleContext;

	public:
		virtual ~IBattleState() = default;
		virtual void Enter(BattleContext& Context) {}
		virtual void Update(BattleContext& Context) = 0;
		virtual void Exit(BattleContext& Context) {}
		virtual FString GetName() const = 0;
	};

	class FMWBSIdle : public IBattleState
	{
	public:
		FMWBSIdle() = default;
		virtual ~FMWBSIdle() = default;
		virtual void Update(BattleContext& Context);
		virtual FString GetName() const { return TEXT("Idle"); }
	};

	class FMWBSBattleBegin : public IBattleState
	{
	public:
		FMWBSBattleBegin() = default;
		virtual ~FMWBSBattleBegin() = default;
		virtual void Enter(BattleContext& Context);
		virtual void Update(BattleContext& Context);
		virtual void Exit(BattleContext& Context) {}
		virtual FString GetName() const { return TEXT("BattleBegin"); }

		void InitializeActionBuff(BattleContext& Context);
	private:
		bool bBattlePrepared = false;
	};

	class FMWBSBattleEnd : public IBattleState
	{
	public:
		FMWBSBattleEnd() = delete;
		FMWBSBattleEnd(EBattleResult Result);
		virtual ~FMWBSBattleEnd() = default;
		virtual void Enter(BattleContext& Context);
		virtual void Update(BattleContext& Context) {}
		virtual FString GetName() const { return TEXT("BattleEnd"); }

	private:
		EBattleResult BattleResult;
	};

	class FMWBSRoundBegin : public IBattleState
	{
	public:
		FMWBSRoundBegin() = default;
		virtual ~FMWBSRoundBegin() = default;
		virtual void Enter(BattleContext& Context);
		virtual void Update(BattleContext& Context);
		virtual FString GetName() const { return TEXT("RoundBegin"); }

		/* Determine the action order for this turn. */
		void UpdateCurrentRoundActionQueue(BattleContext& Context);

		void UpdateActionBuff(BattleContext& Context);
	};

	class FMWBSRoundEnd : public IBattleState
	{
	public:
		FMWBSRoundEnd() = default;
		virtual ~FMWBSRoundEnd() = default;
		virtual void Enter(BattleContext& Context);
		virtual void Update(BattleContext& Context);
		virtual void Exit(BattleContext& Context);
		virtual FString GetName() const { return TEXT("RoundEnd"); }
	};

	class FMWBSTurnBegin : public IBattleState
	{
	public:
		FMWBSTurnBegin() = delete;
		FMWBSTurnBegin(const FMWBattleUnit& NewOwner);
		virtual ~FMWBSTurnBegin() = default;
		virtual void Enter(BattleContext& Context);
		virtual void Update(BattleContext& Context);
		virtual void Exit(BattleContext& Context);
		virtual FString GetName() const { return TEXT("TurnBegin"); }

	protected:
		void OnActionComplete();

	private:
		FMWBattleUnit Owner;
		bool bIsActionComplete = false;
		FDelegateHandle HandleActionComplete;
	};

	class FMWBSTurnEnd : public IBattleState
	{
	public:
		virtual ~FMWBSTurnEnd() = default;
		virtual void Enter(BattleContext& Context);
		virtual void Update(BattleContext& Context);
		virtual void Exit(BattleContext& Context);
		virtual FString GetName() const { return TEXT("TurnEnd"); }

		void CheckShouldEndBattle(BattleContext& Context);
		void CheckShouldRoundEnd(BattleContext& Context);
		void UpdateActionQueueForNextRound(BattleContext& Context);
		void UpdateActionQueueForDeadUnit(BattleContext& Context);

	private:
		bool bIsBattleOver = false;
		EBattleResult Winner = EBattleResult::PlayerWin;
		bool bIsRoundEnd = false;
	};
}

UCLASS()
class UMWBattle : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

public:
	UMWBattle();

	void StartBattle(const TArray<FMWTeam>& InTeams);

	void Tick(float DeltaTime) override;

	virtual TStatId GetStatId() const override;

	bool IsTickable() const override;

	UMWBattleSystem& GetBattleSystem();

private:
	void UpdateBattleState();

private:
	bool bIsTickable;

private:
	/* Called before battle begin. */
	void Initialize();

	/* Called after battle end. */
	void Uninitialize();

	void OnTeamDied(const FMWTeam& Team);

	void OnTeamRevive(const FMWTeam& Team);

public:
	TArray<MWBattle::FMWBattleUnit>& GetCurrentActionQueue() { return CurrActionQueue; }

	TArray<MWBattle::FMWBattleUnit>& GetNextActionQueue() { return NextActionQueue; }

	TArray<EMWBattleActionBuff>& GetActionBuffPool() { return ActionBuffPool; }

	void SetActionBuffPool(const TArray<EMWBattleActionBuff>& NewBuffPool);

	TArray<EMWBattleActionBuff>& GetActiveActionBuffs() { return ActiveActionBuffs; }

	uint32 GetMaxDisplayActiveBuffNo() const { return MaxDisplayActiveBuffNo; }

	TArray<FMWTeam>& GetTeams() { return Teams; }

	void ChangeState(TUniquePtr<MWBattle::IBattleState> NewState);

	void SetCurrentRound(uint32 NewRound) { CurrRound = NewRound; }

	uint32 GetCurrentRound() const { return CurrRound; }

	/* Force to end a battle and decide which force is the winner.
	* @param Winner : 0 = player win, 1 = enemy win, 2 = draw
	*/
	void ForceEndBattle(uint8 Winner);

private:
	/* Cache the teams in the battle. */
	TArray<FMWTeam> Teams;

	/* Cache the action buff can be used in the battle. */
	TArray<EMWBattleActionBuff> ActionBuffPool;

	uint32 CurrRound;

	/* It represents the action order of battle units at the current round. */
	TArray<MWBattle::FMWBattleUnit> CurrActionQueue;

	/* It represents the action order of battle units at the next round. */
	TArray<MWBattle::FMWBattleUnit> NextActionQueue;

	int32 MaxDisplayActiveBuffNo;

	TArray<EMWBattleActionBuff> ActiveActionBuffs;

	TUniquePtr<MWBattle::IBattleState> CurrState;

	/* used to force end a battle. */
	uint8 bForceEndBattle:1;

	EBattleResult BattleResult;

public:
	TObjectPtr<class AMWPlayerController> PC;
};
