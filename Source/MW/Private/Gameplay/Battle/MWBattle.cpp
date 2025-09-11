#include "Gameplay/Battle/MWBattle.h"
#include "Controller/MWPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Gameplay/Battle/MWBattleScenePreparation.h"
#include "Common3DCameraComponent.h"
#include "Input/MWInputUtility.h"
#include "Gameplay/MWGameplayTags.h"
#include "Gameplay/Battle/MWTurnAction.h"
#include "GameDelegates.h"

UE_DISABLE_OPTIMIZATION

FMWActionState FMWActionState::Player	{EMWTeamAlign::Player,	false,	200};
FMWActionState FMWActionState::Enemy	{EMWTeamAlign::Enemy,	false,	100};
FMWActionState FMWActionState::Null	{EMWTeamAlign::Max,		false,	-1};

void MWBattle::MWBSIdle::OnUpdate(float DeltaTime)
{
	this->ChangeState(TEXT("MWBSBattleBegin"));
}


void MWBattle::MWBSBattleBegin::OnEnter()
{
	UMWBattle* context = this->GetOwner();
	check(context != nullptr);

	if (context->GetBattleSystem().OnBattleBegin.IsBound())
	{
		context->GetBattleSystem().OnBattleBegin.Broadcast();
	}

	// prepare scene
	FMWBattleScenePreparation scenePrep;

	FMWBattleSceneParam param;

	param.Teams = context->GetPlayer();
	param.Teams.Append(context->GetEnemy());

	scenePrep.PrepareScene(param);
	// use async task?
	// play cutscene

	bBattlePrepared = true;

	// TODO : REMOVE
	if (context->PC.IsValid())
	{
		context->PC->OnBattleBegin();
	}
}

void MWBattle::MWBSBattleBegin::OnUpdate(float DeltaTime)
{
	if (bBattlePrepared)
	{
		this->ChangeState(TEXT("MWBSRoundBegin"));
	}
}

void MWBattle::MWBSBattleEnd::OnEnter()
{
	// play battle result
		// 游戏结算界面等等
		// wait for battle end
		// destroy battle

	UMWBattle* context = this->GetOwner();

	if (context->GetBattleSystem().OnBattleEnd.IsBound())
	{
		context->GetBattleSystem().OnBattleEnd.Broadcast(BattleResult);
	}

	// TODO : REMOVE
	if (context->PC.IsValid())
	{
		context->PC->OnBattleEnd();
	}
}

void MWBattle::MWBSBattleEnd::OnUpdate(float DeltaTime)
{
}

void MWBattle::MWBSRoundBegin::OnEnter()
{
	UMWBattle* context = this->GetOwner();

	if (context->GetBattleSystem().OnRoundBegin.IsBound())
	{
		context->GetBattleSystem().OnRoundBegin.Broadcast(context->GetCurrentRound());
	}

	// Sort action priority
	auto& actionStates = context->GetActionStates();

	actionStates.Sort([](const FMWActionState& A, const FMWActionState& B)
	{
		return A.Priority > B.Priority;
	});

	// Set the align for the first action
	context->SetCurrentTurnTeamAlign(actionStates[0].Align);
}

void MWBattle::MWBSRoundBegin::OnUpdate(float DeltaTime)
{
	this->ChangeState(TEXT("MWBSTurnBegin"));
}

void MWBattle::MWBSRoundEnd::OnEnter()
{
	UMWBattle* context = this->GetOwner();

	if (context->GetBattleSystem().OnRoundEnd.IsBound())
	{
		context->GetBattleSystem().OnRoundEnd.Broadcast(context->GetCurrentRound());
	}

	ResetActionState();
}

void MWBattle::MWBSRoundEnd::OnUpdate(float DeltaTime)
{
	this->ChangeState(TEXT("MWBSRoundBegin"));
}

void MWBattle::MWBSRoundEnd::OnLeave(bool bShutDown)
{
	UMWBattle* context = this->GetOwner();

	context->SetCurrentRound(context->GetCurrentRound() + 1);
}

void MWBattle::MWBSRoundEnd::ResetActionState()
{
	UMWBattle* context = this->GetOwner();

	auto& actionStates = context->GetActionStates();

	for (auto& state : actionStates)
	{
		state.bActed = false;
	}
}

void MWBattle::MWBSTurnBegin::OnEnter()
{
	if (!DHCleanup.IsValid())
	{
		DHCleanup = FWorldDelegates::OnPostWorldCleanup.AddRaw(this, &MWBattle::MWBSTurnBegin::OnCleanUp);
	}

	UMWBattle* context = this->GetOwner();

	if (!context->GetBattleSystem().OnActionComplete.IsBoundToObject(this))
	{
		DHActionComplete = context->GetBattleSystem().OnActionComplete.AddRaw(this, &MWBattle::MWBSTurnBegin::OnActionComplete);
	}

	const EMWTeamAlign currAlign = context->GetCurrentTurnTeamAlign();

	if (context->GetBattleSystem().OnTurnBegin.IsBound())
	{
		context->GetBattleSystem().OnTurnBegin.Broadcast(currAlign);
	}

	// Change to battle camera
	SetCharacterCameraAsMain();

	// Create Turn Action
	switch (currAlign)
	{
		case EMWTeamAlign::Player:
			TurnAction = NewObject<UMWPlayerTurnAction>(context);
			break;
		case EMWTeamAlign::Enemy:
			TurnAction = NewObject<UMWEnemyTurnAction>(context);
			break;
	}

	TurnAction->Init();

	TurnAction->AddToRoot();
}

void MWBattle::MWBSTurnBegin::OnUpdate(float DeltaTime)
{
	FMWTurnActionData data;

	if (!bIsActionComplete)
	{
		TurnAction->Update(data, bIsActionComplete);
	}

	if (bIsActionComplete)
	{
		UMWBattle* context = this->GetOwner();

		context->GetCurrentTurnActionState().MarkAsActed();

		this->ChangeState(TEXT("MWBSTurnEnd"));
	}
}

void MWBattle::MWBSTurnBegin::OnLeave(bool bShutDown)
{
	bIsActionComplete = false;

	UMWBattle* context = this->GetOwner();

	TurnAction->Uninit();

	TurnAction->RemoveFromRoot();

	TurnAction = nullptr;
}

void MWBattle::MWBSTurnBegin::SetCharacterCameraAsMain()
{
	UMWBattle* context = this->GetOwner();

	const FMWTeam& team = context->GetCurrentTurnTeamAlign() == EMWTeamAlign::Player ? context->GetPlayer().Top() : context->GetEnemy().Top();

	const FMWTeamUnit& leader = team.GetLeader();

	if (leader.Pawn == nullptr)
	{
		return;
	}

	if (auto* pc = UGameplayStatics::GetPlayerController(context->GetWorld(), 0))
	{
		if (auto* camComp = leader.Pawn->FindComponentByClass<UC3DCameraComponent>())
		{
			camComp->SetCameraMode(FGameplayTag::RequestGameplayTag("Camera.Mode.Battle"), true);
		}

		pc->Possess(leader.Pawn);
	}
}

void MWBattle::MWBSTurnBegin::OnDestroy()
{
	if (IsValid(TurnAction))
	{
		TurnAction->RemoveFromRoot();

		TurnAction = nullptr;
	}

	if (DHCleanup.IsValid())
	{
		FWorldDelegates::OnPostWorldCleanup.Remove(DHCleanup);
	}

}

void MWBattle::MWBSTurnBegin::OnActionComplete()
{
	bIsActionComplete = true;
}

void MWBattle::MWBSTurnBegin::OnCleanUp(UWorld* World, bool bSessionEnded, bool bCleanupResources)
{
	if (this->GetOwner() && this->GetOwner()->GetWorld() == World)
	{
		if (this->GetOwner()->GetBattleSystem().OnActionComplete.IsBoundToObject(this))
		{
			this->GetOwner()->GetBattleSystem().OnActionComplete.Remove(DHActionComplete);
		}
	}

	if (IsValid(TurnAction))
	{
		TurnAction->RemoveFromRoot();
	}
}

void MWBattle::MWBSTurnEnd::OnEnter()
{
	UMWBattle* context = this->GetOwner();

	if (context->GetBattleSystem().OnTurnEnd.IsBound())
	{
		context->GetBattleSystem().OnTurnEnd.Broadcast(context->GetCurrentTurnTeamAlign());
	}

	// check if battle end
	CheckShouldEndBattle();

	if (bIsBattleOver)
	{
		return;
	}

	const EMWTeamAlign currAlign = context->GetCurrentTurnTeamAlign();

	// update align of next turn action
	// the ActionStates is already sorted at the round begin,
	// so the one after current align must be the next turn action owner.
	int32 index = context->GetActionStates().IndexOfByKey(context->GetCurrentTurnActionState());

	auto& actionStates = context->GetActionStates();

	// Update current turn align
	if (index + 1 < actionStates.Num())
	{
		context->SetCurrentTurnTeamAlign(actionStates[index + 1].Align);
	}

	// check if round ends
	CheckShouldRoundEnd();
}

void MWBattle::MWBSTurnEnd::OnUpdate(float DeltaTime)
{
	if (bIsBattleOver)
	{
		this->ChangeState(TEXT("MWBSBattleEnd"));
		return;
	}

	if (bIsRoundEnd)
	{
		this->ChangeState(TEXT("MWBSRoundEnd"));
		return;
	}

	// change to next turn
	this->ChangeState(TEXT("MWBSTurnBegin"));
}

void MWBattle::MWBSTurnEnd::CheckShouldEndBattle()
{
	UMWBattle* context = this->GetOwner();

	const TArray<FMWTeam>& playerTeam = context->GetPlayer();

	const TArray<FMWTeam>& enemyTeam = context->GetEnemy();

	// check is game end
	int32 existPlayerNo = 0, existEnemyNo = 0;

	for (auto& unit : playerTeam)
	{
		if (unit.IsAlive())
		{
			++existPlayerNo;
		}
	}

	for (auto& unit : enemyTeam)
	{
		if (unit.IsAlive())
		{
			++existEnemyNo;
		}
	}

	if (existEnemyNo == 0 || existPlayerNo == 0)
	{
		bIsBattleOver = true;

		if (existPlayerNo > 0)
		{
			context->SetBattleResult(EBattleResult::PlayerWin);
		}
		else if (existEnemyNo > 0)
		{
			context->SetBattleResult(EBattleResult::EnemyWin);
		}
		else if (0 == existEnemyNo && 0 == existPlayerNo)
		{
			context->SetBattleResult(EBattleResult::Draw);
		}
	}
}

void MWBattle::MWBSTurnEnd::CheckShouldRoundEnd()
{
	UMWBattle* context = this->GetOwner();

	bIsRoundEnd = true;

	auto& actionStates = context->GetActionStates();

	for (auto& state : actionStates)
	{
		bIsRoundEnd &= state.bActed;
	}
}

UMWBattle::UMWBattle()
{
	//auto fsm = IStandaloneFsm::Create(TEXT("testfsm"), this, new FFsmState<UMWBattle>(TEXT("123")), new FFsmState<UMWBattle>(TEXT("456")));

	//fsm->Start(TEXT("MWBSIdleState"));
	//fsm->Update(0.1f);
	//fsm->ShutDown();
}

void UMWBattle::StartBattle(const FMWBattleData& InData)
{
	check(GetWorld () != nullptr);

	// Init battle
	const uint32 teamNo = InData.Teams.Num();

	bool isValidBattle = false;

	for (uint32 i = 0; i < teamNo; ++i)
	{
		if (InData.Teams[i].GetTeamAlign() == EMWTeamAlign::Player)
		{
			PlayerTeams.Emplace(InData.Teams[i]);
		}
		else if (InData.Teams[i].GetTeamAlign() == EMWTeamAlign::Enemy)
		{
			EnemyTeam.Emplace(InData.Teams[i]);
		}
		
		if (!isValidBattle)
		{
			isValidBattle = InData.Teams[0].GetTeamAlign() != InData.Teams[i].GetTeamAlign();
		}
	}

	check(isValidBattle);

	// Input
	PC = Cast<AMWPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (PC.IsValid())
	{
		UMWInputUtility::DisableMappingContext(PC.Get(), MWGameplayTags::IMCTag_Basic);
	}

	// Delegates
	if (auto* battleSys = UMWBattleSystem::Get(this))
	{
		DHTeamDied = battleSys->OnTeamDied.AddUObject(this, &UMWBattle::OnTeamDied);

		DHTeamRevive = battleSys->OnTeamRevive.AddUObject(this, &UMWBattle::OnTeamRevive);
	}

	// Init state machine
	//CurrState = MakeUnique<MWBattle::FMWBSIdle>();

	Fsm = MakeShared<FFsm<UMWBattle>>(
	TEXT("BattleFsm"), 
	this, 
	new MWBattle::MWBSIdle,
	new MWBattle::MWBSBattleBegin,
	new MWBattle::MWBSBattleEnd,
	new MWBattle::MWBSRoundBegin,
	new MWBattle::MWBSRoundEnd,
	new MWBattle::MWBSTurnBegin,
	new MWBattle::MWBSTurnEnd);

	// Init action state
	for (uint8 i = 0; i < (uint8)EMWTeamAlign::Max; ++i)
	{
		FMWActionState state;
		if (i == (uint8)EMWTeamAlign::Player)
		{
			state = FMWActionState::Player;
		}
		else if (i == (uint8)EMWTeamAlign::Enemy)
		{
			state = FMWActionState::Enemy;
		}
		else
		{
			state.Align = (EMWTeamAlign)i;
			state.bActed = false;
			state.Priority = (int32)EMWTeamAlign::Max - (int32)i;
		}

		ActionStates.Emplace(state);
	}

	bIsTickable = true;

	Fsm->Start(TEXT("MWBSIdle"));
}

void UMWBattle::Tick(float DeltaTime)
{
	if (GetWorld())
	{
		if (Fsm.IsValid())
		{
			auto* currState = Fsm->GetCurrentState();
			Fsm->Update(DeltaTime);
			GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Yellow, FString::Printf(TEXT("state[%s,%f] - round : %d"), *currState->GetName().ToString(), Fsm->GetCurrentStateTime(), CurrRound));
		}
		//UpdateBattleState();
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

void UMWBattle::BeginDestroy()
{
	if(Fsm.IsValid())
	{
		Fsm->ShutDown();
		Fsm.Reset();
	}

	Super::BeginDestroy();
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

FMWActionState& UMWBattle::GetCurrentTurnActionState()
{
	for(auto& state : ActionStates)
	{
		if (state.Align == GetCurrentTurnTeamAlign())
		{
			return state;
		}
	}

	return FMWActionState::Null;
}

void UMWBattle::ForceEndBattle(EBattleResult InWinner)
{
	bForceEndBattle = true;
	BattleResult = InWinner;
}

void UMWBattle::Initialize()
{
}

void UMWBattle::Uninitialize()
{
}

void UMWBattle::OnTeamDied(const FMWTeam& Team)
{
	int32 index = PlayerTeams.Find(Team);
	if (index != INDEX_NONE)
	{
		PlayerTeams[index].SetAlive(false);
	}
}

void UMWBattle::OnTeamRevive(const FMWTeam& Team)
{
	int32 index = PlayerTeams.Find(Team);
	if (index != INDEX_NONE)
	{
		PlayerTeams[index].SetAlive(true);
	}
}

UMWBattleSystem& UMWBattle::GetBattleSystem()
{
	return  *UMWBattleSystem::Get(this);
}
UE_ENABLE_OPTIMIZATION

