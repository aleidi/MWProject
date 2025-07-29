#include "Gameplay/Battle/MWBattle.h"
#include "Controller/MWPlayerController.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Gameplay/Battle/MWBattleScenePreparation.h"
#include "Common3DCameraComponent.h"

UE_DISABLE_OPTIMIZATION

UMWBattle::UMWBattle()
	: bIsTickable(false)
	, CurrRound(1)
	, MaxDisplayActiveBuffNo(5)
	, bForceEndBattle(false)
	, bPlayerTurn(true)
{
}

void UMWBattle::StartBattle(const FMWBattleData& InData)
{
	check(GetWorld () != nullptr);

	if (IsValid(PC))
	{
		PC = Cast<AMWPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	}

	if (auto* battleSys = UMWBattleSystem::Get(this))
	{
		battleSys->OnTeamDied.AddUObject(this, &UMWBattle::OnTeamDied);
		battleSys->OnTeamRevive.AddUObject(this, &UMWBattle::OnTeamRevive);
	}

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

void UMWBattle::ChangeToNewTurn()
{
	bPlayerTurn = !bPlayerTurn;
}

void UMWBattle::ForceEndBattle(EBattleResult Winner)
{
	bForceEndBattle = true;
	BattleResult = Winner;
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

void MWBattle::FMWBSTurnBegin::Enter(BattleContext& Context)
{
	if (Context.GetBattleSystem().OnTurnBegin.IsBound())
	{
		Context.GetBattleSystem().OnTurnBegin.Broadcast(Context.GetCurrentTurnTeamAlign());
	}
	
	// TODO : REMOVE
	if (Context.GetBattleSystem().OnCommandBattleBegin.IsBound())
	{
		Context.GetBattleSystem().OnCommandBattleBegin.Broadcast();
	}

	HandleActionComplete = Context.GetBattleSystem().OnActionComplete.AddRaw(this, &MWBattle::FMWBSTurnBegin::OnActionComplete);

	// Change to battle camera
	SetCharacterCameraAsMain(Context);
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
	HandleActionComplete.Reset();
}

void MWBattle::FMWBSTurnBegin::OnActionComplete()
{
	bIsActionComplete = true;
}

void MWBattle::FMWBSTurnBegin::SetCharacterCameraAsMain(UMWBattle& Context)
{
	
	
	const FMWTeam& team = Context.GetCurrentTurnTeamAlign() == EMWTeamAlign::Player ? Context.GetPlayer().Top() : Context.GetEnemy().Top();

	const FMWTeamUnit& leader = team.GetLeader();

	if (leader.Pawn == nullptr)
	{
		return;
	}

	if (auto* pc = UGameplayStatics::GetPlayerController(Context.GetWorld(), 0))
	{
		if (auto* camComp = leader.Pawn->FindComponentByClass<UC3DCameraComponent>())
		{
			camComp->SetCameraMode(FGameplayTag::RequestGameplayTag("Camera.Mode.Battle"), true);
		}

		pc->Possess(leader.Pawn);
	}
}

void MWBattle::FMWBSTurnEnd::Enter(BattleContext& Context)
{
	if (Context.GetBattleSystem().OnTurnEnd.IsBound())
	{
		Context.GetBattleSystem().OnTurnEnd.Broadcast(Context.GetCurrentTurnTeamAlign());
	}

	// TODO : REMOVE
	if (Context.GetBattleSystem().OnCommandBattleEnd.IsBound())
	{
		Context.GetBattleSystem().OnCommandBattleEnd.Broadcast();
	}

	// check if battle end
	CheckShouldEndBattle(Context);

	if (bIsBattleOver)
	{
		return;
	}

	// check if round end
	CheckShouldRoundEnd(Context);

	// update team align of next turn
	Context.ChangeToNewTurn();
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
	Context.ChangeState(MakeUnique<MWBattle::FMWBSTurnBegin>());
}

void MWBattle::FMWBSTurnEnd::Exit(BattleContext& Context)
{
}

void MWBattle::FMWBSTurnEnd::CheckShouldEndBattle(BattleContext& Context)
{
	const TArray<FMWTeam>& playerTeam = Context.GetPlayer();

	const TArray<FMWTeam>& enemyTeam = Context.GetEnemy();

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
	// player first, then enemy.
	// after enemy finish action, round over.
	bIsRoundEnd = Context.GetCurrentTurnTeamAlign() == EMWTeamAlign::Enemy ? true : false;
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

	// TODO : REMOVE
	if (IsValid(Context.PC))
	{
		Context.PC->OnBattleEnd();
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
	if (Context.GetBattleSystem().OnRoundBegin.IsBound())
	{
		Context.GetBattleSystem().OnRoundBegin.Broadcast(Context.GetCurrentRound());
	}
}

void MWBattle::FMWBSRoundBegin::Update(BattleContext& Context)
{
	Context.ChangeState(MakeUnique<MWBattle::FMWBSTurnBegin>());
}

void MWBattle::FMWBSBattleBegin::Enter(BattleContext& Context)
{
	if (Context.GetBattleSystem().OnBattleBegin.IsBound())
	{
		Context.GetBattleSystem().OnBattleBegin.Broadcast();
	}

	// prepare scene
	FMWBattleScenePreparation scenePrep;

	FMWBattleSceneParam param;

	param.Teams = Context.GetPlayer();
	param.Teams.Append(Context.GetEnemy());

	scenePrep.PrepareScene(param);
	// use async task?
	// play cutscene

	bBattlePrepared = true;

	// TODO : REMOVE
	if (IsValid(Context.PC))
	{
		Context.PC->OnBattleBegin();
	}
}

void MWBattle::FMWBSBattleBegin::Update(BattleContext& Context)
{
	if (bBattlePrepared)
	{
		Context.ChangeState(MakeUnique<MWBattle::FMWBSRoundBegin>());
	}
}

void MWBattle::FMWBSIdle::Update(BattleContext& Context)
{
	Context.ChangeState(MakeUnique<MWBattle::FMWBSBattleBegin>());
}
UE_ENABLE_OPTIMIZATION