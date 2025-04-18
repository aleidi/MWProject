#include "Gameplay/Battle/MWBattle.h"
UE_DISABLE_OPTIMIZATION

UMWBattle::UMWBattle()
	: bIsTickable(false)
	, CurrRound(1)
	, MaxDisplayActiveBuffNo(5)
	, bForceEndBattle(false)
{
}

void UMWBattle::StartBattle(const TArray<FMWTeam>& InTeams)
{
	check(GetWorld () != nullptr);

	if (auto* battleSys = GetWorld()->GetSubsystem<UMWBattleSystem>())
	{
		battleSys->OnTeamDied.AddUObject(this, &UMWBattle::OnTeamDied);
		battleSys->OnTeamRevive.AddUObject(this, &UMWBattle::OnTeamRevive);
	}

	const uint32 teamNo = InTeams.Num();
	check(teamNo > 0);

	bool isValidBattle = false;
	for (uint32 i = 1; i < teamNo; ++i)
	{
		if (InTeams[0].GetTeamType() != InTeams[i].GetTeamType())
		{
			isValidBattle = true;
			break;
		}
	}

	check(isValidBattle);

	CurrActionQueue.Reserve(teamNo * 2);
	NextActionQueue.Reserve(teamNo * 2);

	Teams = InTeams;
	for (auto& team : Teams)
	{
		MWBattle::FMWBattleUnit unit{ team };
		NextActionQueue.Emplace(unit);
	}

	CurrState = MakeUnique<MWBattle::FMWBSIdle>();

	bIsTickable = true;
}

void UMWBattle::Tick(float DeltaTime)
{
	if (GetWorld())
	{
		UpdateBattleState();
	}
}

TStatId UMWBattle::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UMWBattle, STATGROUP_Tickables);
}

bool UMWBattle::IsTickable() const
{
	return bIsTickable;
}

void UMWBattle::UpdateBattleState()
{
	if (bForceEndBattle && CurrState->GetName() != TEXT("BattleEnd"))
	{
		ChangeState(MakeUnique<MWBattle::FMWBSBattleEnd>(BattleResult));
	}

	if (CurrState.IsValid())
	{
		CurrState->Update(*this);
		GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Yellow, FString::Printf(TEXT("%s - round : %d"), *CurrState->GetName(), CurrRound));
	}

	if (CurrState->GetName() == TEXT("BattleEnd"))
	{
		CurrState.Reset();
		bIsTickable = false;
	}
}

void UMWBattle::SetActionBuffPool(const TArray<EMWBattleActionBuff>& NewBuffPool)
{
	ActionBuffPool = NewBuffPool;
}

void UMWBattle::ChangeState(TUniquePtr<MWBattle::IBattleState> NewState)
{
	if (CurrState.IsValid())
	{
		CurrState->Exit(*this);
	}

	CurrState = MoveTemp(NewState);

	if (CurrState.IsValid())
	{
		CurrState->Enter(*this);
	}
}

void UMWBattle::ForceEndBattle(uint8 Winner)
{
	bForceEndBattle = true;
	BattleResult = (EBattleResult)FMath::Clamp(Winner, (int32)EBattleResult::PlayerWin, (int32)EBattleResult::Draw);
}

void UMWBattle::Initialize()
{
}

void UMWBattle::Uninitialize()
{
}

void UMWBattle::OnTeamDied(const FMWTeam& Team)
{
	int32 index = Teams.Find(Team);
	if (index != INDEX_NONE)
	{
		Teams[index].SetAlive(false);
	}
}

void UMWBattle::OnTeamRevive(const FMWTeam& Team)
{
	int32 index = Teams.Find(Team);
	if (index != INDEX_NONE)
	{
		Teams[index].SetAlive(true);
	}
}

UMWBattleSystem& UMWBattle::GetBattleSystem()
{
	return  *GetWorld()->GetSubsystem<UMWBattleSystem>();
}

void MWBattle::FMWBSTurnBegin::Enter(BattleContext& Context)
{
	check(Context.GetCurrentActionQueue().Num() > 0 && Context.GetCurrentActionQueue()[0].Team.IsAlive());

	Context.GetBattleSystem().OnTurnBegin.Broadcast(Context.GetCurrentActionQueue()[0].Team);

	HandleActionComplete = Context.GetBattleSystem().OnActionComplete.AddRaw(this, &MWBattle::FMWBSTurnBegin::OnActionComplete);

	Async(EAsyncExecution::Thread, [this](){

		FPlatformProcess::Sleep(5.0f);

		// 然后继续执行你想在等待结束后进行的操作
		AsyncTask(ENamedThreads::GameThread, [this]()
		{
			bIsActionComplete = true;
		});

	});
}

void MWBattle::FMWBSTurnBegin::Update(BattleContext& Context)
{
	if (bIsActionComplete)
	{
		Context.ChangeState(MakeUnique<MWBattle::FMWBSTurnEnd>());
	}
}

void MWBattle::FMWBSTurnBegin::Exit(BattleContext& Context)
{
	Context.GetBattleSystem().OnActionComplete.Remove(HandleActionComplete);
}

void MWBattle::FMWBSTurnBegin::OnActionComplete()
{
	bIsActionComplete = true;
}

MWBattle::FMWBSTurnBegin::FMWBSTurnBegin(const MWBattle::FMWBattleUnit& NewOwner)
{
	Owner = NewOwner;
}

void MWBattle::FMWBSTurnEnd::Enter(BattleContext& Context)
{
	// remove dead unit first
	UpdateActionQueueForDeadUnit(Context);

	// check if battle end
	CheckShouldEndBattle(Context);
	if (bIsBattleOver)
	{
		return;
	}

	// update action queue for next round
	UpdateActionQueueForNextRound(Context);

	// check if round end
	CheckShouldRoundEnd(Context);
}

void MWBattle::FMWBSTurnEnd::Update(BattleContext& Context)
{
	if (bIsBattleOver)
	{
		Context.ChangeState(MakeUnique<MWBattle::FMWBSBattleEnd>(Winner));
		return;
	}

	if (bIsRoundEnd)
	{
		Context.ChangeState(MakeUnique<MWBattle::FMWBSRoundEnd>());
		return;
	}

	// change to next turn
	Context.ChangeState(MakeUnique<MWBattle::FMWBSTurnBegin>(Context.GetCurrentActionQueue()[0]));
}

void MWBattle::FMWBSTurnEnd::Exit(BattleContext& Context)
{
}

void MWBattle::FMWBSTurnEnd::CheckShouldEndBattle(BattleContext& Context)
{
	const TArray<FMWBattleUnit>& currActQueue = Context.GetCurrentActionQueue();
	const TArray<FMWBattleUnit>& nextActQueue = Context.GetNextActionQueue();

	// check is game end
	int32 existPlayerNo = 0, existEnemyNo = 0;

	for (auto& unit : currActQueue)
	{
		if (unit.Team.GetTeamType() == EMWTeamType::Player)
		{
			++existPlayerNo;
		}
		else if (unit.Team.GetTeamType() == EMWTeamType::Enemy)
		{
			++existEnemyNo;
		}
	}

	for (auto& unit : nextActQueue)
	{
		if (unit.Team.GetTeamType() == EMWTeamType::Player)
		{
			++existPlayerNo;
		}
		else if (unit.Team.GetTeamType() == EMWTeamType::Enemy)
		{
			++existEnemyNo;
		}
	}

	if (existEnemyNo == 0 || existPlayerNo == 0)
	{
		bIsBattleOver = true;

		if (existPlayerNo > 0)
		{
			Winner = EBattleResult::PlayerWin;
		}
		else if (existEnemyNo > 0)
		{
			Winner = EBattleResult::EnemyWin;
		}
		else if (0 == existEnemyNo && 0 == existPlayerNo)
		{
			Winner = EBattleResult::Draw;
		}
	}
}

void MWBattle::FMWBSTurnEnd::CheckShouldRoundEnd(BattleContext& Context)
{
	bIsRoundEnd = Context.GetCurrentActionQueue().Num() == 0;
}

void MWBattle::FMWBSTurnEnd::UpdateActionQueueForNextRound(BattleContext& Context)
{
	TArray<FMWBattleUnit>& nextActQueue = Context.GetNextActionQueue();

	FMWBattleUnit unit = Context.GetCurrentActionQueue().Pop(false);
	unit.WaitTime += WAIT_TIME_DEFAULT;

	nextActQueue.Emplace(unit);
}

void MWBattle::FMWBSTurnEnd::UpdateActionQueueForDeadUnit(BattleContext& Context)
{
	auto RemoveDeadUnit = [](TArray<FMWBattleUnit>& Queue)
	{
		Queue.RemoveAll([](const FMWBattleUnit& Unit)
		{
			return !Unit.Team.IsAlive();
		});
	};

	TArray<FMWBattleUnit>& currActQueue = Context.GetCurrentActionQueue();
	TArray<FMWBattleUnit>& nextActQueue = Context.GetNextActionQueue();

	RemoveDeadUnit(currActQueue);
	RemoveDeadUnit(nextActQueue);

}

MWBattle::FMWBSBattleEnd::FMWBSBattleEnd(EBattleResult Result)
{
	BattleResult = Result;
}

void MWBattle::FMWBSBattleEnd::Enter(BattleContext& Context)
{
	// play battle result
	// 游戏结算界面等等
	// wait for battle end
	// destroy battle

	if (Context.GetBattleSystem().OnBattleEnd.IsBound())
	{
		Context.GetBattleSystem().OnBattleEnd.Broadcast(BattleResult);
	}
}

void MWBattle::FMWBSRoundEnd::Enter(BattleContext& Context)
{
	if (Context.GetBattleSystem().OnRoundEnd.IsBound())
	{
		Context.GetBattleSystem().OnRoundEnd.Broadcast(Context.GetCurrentRound());
	}
}

void MWBattle::FMWBSRoundEnd::Update(BattleContext& Context)
{
	Context.ChangeState(MakeUnique<MWBattle::FMWBSRoundBegin>());
}

void MWBattle::FMWBSRoundEnd::Exit(BattleContext& Context)
{
	Context.SetCurrentRound(Context.GetCurrentRound() + 1);
}

void MWBattle::FMWBSRoundBegin::Enter(BattleContext& Context)
{
	UpdateCurrentRoundActionQueue(Context);

	UpdateActionBuff(Context);

	if (Context.GetBattleSystem().OnRoundBegin.IsBound())
	{
		Context.GetBattleSystem().OnRoundBegin.Broadcast(Context.GetCurrentRound());
	}
}

void MWBattle::FMWBSRoundBegin::Update(BattleContext& Context)
{
	Context.ChangeState(MakeUnique<MWBattle::FMWBSTurnBegin>(Context.GetCurrentActionQueue()[0]));
}

void MWBattle::FMWBSRoundBegin::UpdateCurrentRoundActionQueue(BattleContext& Context)
{
	TArray<FMWBattleUnit>& nextActQueue = Context.GetNextActionQueue();
	TArray<FMWBattleUnit>& currActQueue = Context.GetCurrentActionQueue();

	currActQueue = nextActQueue;
	nextActQueue.Reset();

	// find revive battle units
	const TArray<FMWTeam>& teams = Context.GetTeams();
	for (const auto& team : teams)
	{
		if (team.IsAlive())
		{
			FMWBattleUnit unit{team};
			if (!currActQueue.Contains(unit))
			{
				currActQueue.Emplace(unit);
			}
		}
	}

	bool bUpdateWaitTime = true;
	while (bUpdateWaitTime)
	{
		// update wait time of each battle unit, if wait time <= 0, then update is complete
		for (auto& battleUnit : currActQueue)
		{
			battleUnit.WaitTime -= battleUnit.Team.GetTeamSpeed();
			if (battleUnit.WaitTime <= 0)
			{
				bUpdateWaitTime = false;
			}
		}
	}

	currActQueue.Sort([](const MWBattle::FMWBattleUnit& A, const MWBattle::FMWBattleUnit& B)
	{
		return A.WaitTime < B.WaitTime;
	});

	TArray<FMWTeam> newTeamQueue;
	const uint32 queueNo = currActQueue.Num();
	for (uint32 i = 0; i < queueNo; ++i)
	{
		newTeamQueue.Emplace(currActQueue[i].Team);
	}

	// Broadcast new action queue
	if (Context.GetBattleSystem().OnActionQueueUpdate.IsBound())
	{
		Context.GetBattleSystem().OnActionQueueUpdate.Broadcast(newTeamQueue);
	}
}

void MWBattle::FMWBSRoundBegin::UpdateActionBuff(BattleContext& Context)
{
	TArray<EMWBattleActionBuff>& activeActionBuffs = Context.GetActiveActionBuffs();
	TArray<EMWBattleActionBuff>& actionBuffPool = Context.GetActionBuffPool();

	activeActionBuffs.Pop(false);

	int32 id = FMath::RandRange(0, actionBuffPool.Num() - 1);

	activeActionBuffs.Push(actionBuffPool[id]);

	// broadcast new action buff
	if (Context.GetBattleSystem().OnActionBuffUpdate.IsBound())
	{
		Context.GetBattleSystem().OnActionBuffUpdate.Broadcast(actionBuffPool[id]);
	}
}

void MWBattle::FMWBSBattleBegin::Enter(BattleContext& Context)
{
	if (Context.GetBattleSystem().OnBattleBegin.IsBound())
	{
		Context.GetBattleSystem().OnBattleBegin.Broadcast();

	}

	// prepare scene
	// use async task?
	// play cutscene

	InitializeActionBuff(Context);

	bBattlePrepared = true;
}

void MWBattle::FMWBSBattleBegin::Update(BattleContext& Context)
{
	if (bBattlePrepared)
	{
		Context.ChangeState(MakeUnique<MWBattle::FMWBSRoundBegin>());
	}
}

void MWBattle::FMWBSBattleBegin::InitializeActionBuff(BattleContext& Context)
{
	TArray<EMWBattleActionBuff>& actionBuffPool = Context.GetActionBuffPool();

	// Get all action buff can be used here : TODO
	TArray<EMWBattleActionBuff> buffPool;
	buffPool.Emplace(EMWBattleActionBuff::Attack);
	buffPool.Emplace(EMWBattleActionBuff::Idle);
	buffPool.Emplace(EMWBattleActionBuff::Heal);
	Context.SetActionBuffPool(buffPool);

	TArray<EMWBattleActionBuff>& activeActionBuffs = Context.GetActiveActionBuffs();
	const uint32 maxDisplayActiveBuffNo = Context.GetMaxDisplayActiveBuffNo();

	activeActionBuffs.Reset();

	const uint32 randMax = actionBuffPool.Num() - 1;

	const bool isDeleBound = Context.GetBattleSystem().OnActionBuffUpdate.IsBound();

	for (uint32 i = 0; i < maxDisplayActiveBuffNo; ++i)
	{
		int32 id = FMath::RandRange(0, randMax);

		activeActionBuffs.Push(actionBuffPool[id]);

		if (isDeleBound)
		{
			Context.GetBattleSystem().OnActionBuffUpdate.Broadcast(actionBuffPool[id]);
		}
	}
}

void MWBattle::FMWBSIdle::Update(BattleContext& Context)
{
	Context.ChangeState(MakeUnique<MWBattle::FMWBSBattleBegin>());
}
UE_ENABLE_OPTIMIZATION