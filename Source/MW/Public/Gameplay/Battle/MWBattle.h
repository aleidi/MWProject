#pragma once

#include "CoreMinimal.h"
#include "Define/MWDefineGameplay.h"
#include "MWLogChannels.h"
#include "Gameplay/Battle/MWBattleSystem.h"
#include "MWBattle.generated.h"

namespace MWBattle
{
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
		FMWBSTurnBegin() = default;
		virtual ~FMWBSTurnBegin() = default;
		virtual void Enter(BattleContext& Context);
		virtual void Update(BattleContext& Context);
		virtual void Exit(BattleContext& Context);
		virtual FString GetName() const { return TEXT("TurnBegin"); }

	protected:
		void OnActionComplete();

		void SetCharacterCameraAsMain(UMWBattle& Context);

	private:
		bool bIsActionComplete = false;
		FDelegateHandle HandleActionComplete;
	};

	class FMWBSTurnEnd : public IBattleState
	{
	public:
		FMWBSTurnEnd() = default;
		virtual ~FMWBSTurnEnd() = default;
		virtual void Enter(BattleContext& Context);
		virtual void Update(BattleContext& Context);
		virtual void Exit(BattleContext& Context);
		virtual FString GetName() const { return TEXT("TurnEnd"); }

		void CheckShouldEndBattle(BattleContext& Context);
		void CheckShouldRoundEnd(BattleContext& Context);

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
	
	friend class MWBattle::FMWBSIdle;
	friend class MWBattle::FMWBSBattleBegin;
	friend class MWBattle::FMWBSBattleEnd;
	friend class MWBattle::FMWBSRoundBegin;
	friend class MWBattle::FMWBSRoundEnd;
	friend class MWBattle::FMWBSTurnBegin;
	friend class MWBattle::FMWBSTurnEnd;

public:
	UMWBattle();

	void StartBattle(const FMWBattleData& InData);

	void Tick(float DeltaTime) override;

	virtual TStatId GetStatId() const override;

	bool IsTickable() const override;

private:
	UMWBattleSystem& GetBattleSystem();

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

	void ChangeState(TUniquePtr<MWBattle::IBattleState> NewState);

	void SetCurrentRound(uint32 NewRound) { CurrRound = NewRound; }

	void SetActionBuffPool(const TArray<EMWBattleActionBuff>& NewBuffPool);

public:
	const TArray<FMWTeam>& GetPlayer() const { return PlayerTeams; }

	const TArray<FMWTeam>& GetEnemy() const { return EnemyTeam; }

	TArray<EMWBattleActionBuff>& GetActionBuffPool() { return ActionBuffPool; }

	TArray<EMWBattleActionBuff>& GetActiveActionBuffs() { return ActiveActionBuffs; }

	uint32 GetMaxDisplayActiveBuffNo() const { return MaxDisplayActiveBuffNo; }

	uint32 GetCurrentRound() const { return CurrRound; }

	EMWTeamAlign GetCurrentTurnTeamAlign() const { return bPlayerTurn ? EMWTeamAlign::Player : EMWTeamAlign::Enemy; }

	void ChangeToNewTurn();

	/* Force to end a battle and decide which force is the winner.
	* @param Winner : 0 = player win, 1 = enemy win, 2 = draw
	*/
	void ForceEndBattle(EBattleResult Winner);

private:
	/* Cache the teams in the battle. */
	TArray<FMWTeam> PlayerTeams;

	TArray<FMWTeam> EnemyTeam;

	/* Cache the action buff can be used in the battle. */
	TArray<EMWBattleActionBuff> ActionBuffPool;

	uint32 CurrRound;

	int32 MaxDisplayActiveBuffNo;

	TArray<EMWBattleActionBuff> ActiveActionBuffs;

	TUniquePtr<MWBattle::IBattleState> CurrState;

	EBattleResult BattleResult;

	/* used to force end a battle. */
	bool bForceEndBattle;

	/* Player's Turn or Enemy's Turn.*/
	bool bPlayerTurn;

public:
	TObjectPtr<class AMWPlayerController> PC;
};
