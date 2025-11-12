#pragma once

#include "CoreMinimal.h"
#include "Define/MWDefineGameplay.h"
#include "MWLogChannels.h"
#include "Gameplay/Battle/MWBattleSystem.h"
#include "Util/FsmUtil.h"
#include "MWBattle.generated.h"

// Forward Declare
class UMWTurnAction;
class UMWBattleUnit;

struct FMWActionState
{
	EMWTeamAlign Align = EMWTeamAlign::Max;

	/* Used to check the turn of align if ended. */
	bool bActed = false;

	/* Used to determine the action order of the align. */
	int32 Priority = INDEX_NONE;

	static FMWActionState Player;
	static FMWActionState Enemy;
	static FMWActionState Null;

	bool IsValid() const { return Align != EMWTeamAlign::Max && Priority != INDEX_NONE; }

	void Reset() { *this = FMWActionState::Null; }

	void MarkAsActed() { bActed = true; }

	bool operator==(const FMWActionState& Other)
	{
		return Align == Other.Align && bActed == Other.bActed && Priority == Other.Priority;
	}
};

FORCEINLINE bool operator==(const FMWActionState& A, const FMWActionState& B)
{
	return A.Align == B.Align && A.bActed == B.bActed && A.Priority == B.Priority;
}

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

	DECLARE_FSM_STATE_START(UMWBattle, MWBSIdle)
	virtual void OnUpdate(float DeltaTime) override;
	DECLARE_FSM_STATE_END()

	DECLARE_FSM_STATE_START(UMWBattle, MWBSBattleBegin)
	virtual void OnEnter() override;
	virtual void OnUpdate(float DeltaTime) override;

private:
	bool bBattlePrepared = false;
	DECLARE_FSM_STATE_END()

	DECLARE_FSM_STATE_START(UMWBattle, MWBSBattleEnd)
	virtual void OnEnter() override;
	virtual void OnUpdate(float DeltaTime) override;

private:
	EBattleResult BattleResult = EBattleResult::Draw;
	DECLARE_FSM_STATE_END()

	DECLARE_FSM_STATE_START(UMWBattle, MWBSRoundBegin)
	virtual void OnEnter() override;
	virtual void OnUpdate(float DeltaTime) override;
	DECLARE_FSM_STATE_END()

	DECLARE_FSM_STATE_START(UMWBattle, MWBSRoundEnd)
	virtual void OnEnter() override;
	virtual void OnUpdate(float DeltaTime) override;
	virtual void OnLeave(bool bShutDown) override;
	void ResetActionState();
	DECLARE_FSM_STATE_END()

	DECLARE_FSM_STATE_START(UMWBattle, MWBSTurnBegin)
	virtual void OnEnter() override;
	virtual void OnUpdate(float DeltaTime) override;
	virtual void OnLeave(bool bShutDown) override;
	virtual void OnDestroy() override;
	void SetCharacterCameraAsMain();
	void OnActionComplete();

	bool bIsActionComplete = false;
	FDelegateHandle DHActionComplete;
	TObjectPtr<UMWTurnAction> TurnAction;

	FDelegateHandle DHCleanup;
	void OnCleanUp(UWorld* World, bool bSessionEnded, bool bCleanupResources);
	DECLARE_FSM_STATE_END()

	DECLARE_FSM_STATE_START(UMWBattle, MWBSTurnEnd)
	virtual void OnEnter() override;
	virtual void OnUpdate(float DeltaTime) override;
	void CheckShouldEndBattle();
	void CheckShouldRoundEnd();

	bool bIsBattleOver = false;
	bool bIsRoundEnd = false;
	DECLARE_FSM_STATE_END()
}

UCLASS()
class UMWBattle : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

public:
	UMWBattle();

	void StartBattle(const FMWBattleData& InData);

	void Tick(float DeltaTime) override;

	virtual TStatId GetStatId() const override;

	bool IsTickable() const override;

	virtual void BeginDestroy() override;

public:
	UMWBattleSystem& GetBattleSystem();

private:
	bool bIsTickable = false;

private:
	/* Called before battle begin. */
	void Initialize();

	/* Called after battle end. */
	void Uninitialize();

	void InitBattleUnits(const FMWBattleData& InData);

public:
	const FMWTeam& GetPlayerTeam() const { return PlayerTeam; }

	const FMWTeam& GetEnemyTeam() const { return EnemyTeam; }

	void SetActionBuffPool(const TArray<EMWBattleActionBuff>& NewBuffPool);

	TArray<EMWBattleActionBuff>& GetActionBuffPool() { return ActionBuffPool; }

	TArray<EMWBattleActionBuff>& GetActiveActionBuffs() { return ActiveActionBuffs; }

	uint32 GetMaxDisplayActiveBuffNo() const { return MaxDisplayActiveBuffNo; }

	void SetCurrentRound(uint32 NewRound) { CurrRound = NewRound; }

	uint32 GetCurrentRound() const { return CurrRound; }

	EMWTeamAlign GetCurrentTurnTeamAlign() const { return CurrentTurnAlign; }

	void SetCurrentTurnTeamAlign(EMWTeamAlign NewAlign) { CurrentTurnAlign = NewAlign; }

	FMWActionState& GetCurrentTurnActionState();

	TArray<FMWActionState>& GetActionStates() { return ActionStates; }

	void SetBattleResult(EBattleResult InResult) { BattleResult = InResult; }

	EBattleResult GetBattleResult() const { return BattleResult; }

	/* Force to end a battle and decide which force is the winner.
	* @param Winner : 0 = player win, 1 = enemy win, 2 = draw
	*/
	void ForceEndBattle(EBattleResult InWinner);

private:
	/* Cache the teams in the battle. */
	FMWTeam PlayerTeam;

	FMWTeam EnemyTeam;

	/* Cache the action buff can be used in the battle. */
	TArray<EMWBattleActionBuff> ActionBuffPool;

	uint32 CurrRound = 0;

	int32 MaxDisplayActiveBuffNo = 5;

	TArray<EMWBattleActionBuff> ActiveActionBuffs;

	TUniquePtr<MWBattle::IBattleState> CurrState;

	EBattleResult BattleResult = EBattleResult::Draw;

	/* used to force end a battle. */
	bool bForceEndBattle = false;

	/* The align is acting in current turn. */
	EMWTeamAlign CurrentTurnAlign;

	/* Record the action state of each align. Used to check if round should be ended. */
	TArray<FMWActionState> ActionStates;

	FDelegateHandle DHTeamDied;

	FDelegateHandle DHTeamRevive;

public:
	TWeakObjectPtr<class AMWPlayerController> PC;

private:
	TSharedPtr<FFsm<UMWBattle>> Fsm;
};