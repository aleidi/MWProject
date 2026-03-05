#include "Gameplay/Battle/MWCommandBattle.h"
#include "Controller/MWPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Gameplay/Battle/MWBattleScenePreparation.h"
#include "Common3DCameraComponent.h"
#include "Input/MWInputUtility.h"
#include "Gameplay/MWGameplayTags.h"
#include "Gameplay/Battle/MWActionExecutor.h"
#include "GameDelegates.h"
#include "Gameplay/Battle/BattleUnit/MWBattleUnitAvatar.h"

#if WITH_EDITOR
#include "Editor.h"
#endif

UE_DISABLE_OPTIMIZATION

FMWActionState FMWActionState::Player	{EMWTeamAlign::Player,	false,	200};
FMWActionState FMWActionState::Enemy	{EMWTeamAlign::Enemy,	false,	100};
FMWActionState FMWActionState::Null		{EMWTeamAlign::Max,		false,	-1};

void MWCommandBattle::MWBSIdle::OnUpdate(float DeltaTime)
{
	this->ChangeState(TEXT("MWBSBattleBegin"));
}

void MWCommandBattle::MWBSBattleBegin::OnEnter()
{
	UMWCommandBattle* context = this->GetOwner();
	check(context != nullptr);

	//if (context->GetBattleSystem().OnBattleBegin.IsBound())
	//{
	//	context->GetBattleSystem().OnBattleBegin.Broadcast();
	//}

	// prepare scene
	FMWBattleScenePreparation scenePrep;

	FMWBattleSceneParam param;
	param.EnemyTeam = context->GetEnemyTeam();
	param.PlayerTeam = context->GetPlayerTeam();

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

void MWCommandBattle::MWBSBattleBegin::OnUpdate(float DeltaTime)
{
	if (bBattlePrepared)
	{
		this->ChangeState(TEXT("MWBSRoundBegin"));
	}
}

void MWCommandBattle::MWBSBattleEnd::OnEnter()
{
	// play battle result
	// 游戏结算界面等等
	// wait for battle end
	// destroy battle

	UMWCommandBattle* context = this->GetOwner();

	//if (context->GetBattleSystem().OnBattleEnd.IsBound())
	//{
	//	context->GetBattleSystem().OnBattleEnd.Broadcast(BattleResult);
	//}

	// TODO : REMOVE
	if (context->PC.IsValid())
	{
		context->PC->OnBattleEnd();
	}
}

void MWCommandBattle::MWBSBattleEnd::OnUpdate(float DeltaTime)
{
	GetFsm()->Stop(true);
}

void MWCommandBattle::MWBSRoundBegin::OnEnter()
{
	UMWCommandBattle* context = this->GetOwner();

	//if (context->GetBattleSystem().OnRoundBegin.IsBound())
	//{
	//	context->GetBattleSystem().OnRoundBegin.Broadcast(context->GetCurrentRound());
	//}

	// Sort action priority
	auto& actionStates = context->GetActionStates();

	actionStates.Sort([](const FMWActionState& A, const FMWActionState& B)
	{
		return A.Priority > B.Priority;
	});

	// Set the align for the first action
	context->SetCurrentTurnTeamAlign(actionStates[0].Align);
}

void MWCommandBattle::MWBSRoundBegin::OnUpdate(float DeltaTime)
{
	this->ChangeState(TEXT("MWBSTurnBegin"));
}

void MWCommandBattle::MWBSRoundEnd::OnEnter()
{
	UMWCommandBattle* context = this->GetOwner();

	//if (context->GetBattleSystem().OnRoundEnd.IsBound())
	//{
	//	context->GetBattleSystem().OnRoundEnd.Broadcast(context->GetCurrentRound());
	//}

	ResetActionState();
}

void MWCommandBattle::MWBSRoundEnd::OnUpdate(float DeltaTime)
{
	this->ChangeState(TEXT("MWBSRoundBegin"));
}

void MWCommandBattle::MWBSRoundEnd::OnLeave(bool bShutDown)
{
	UMWCommandBattle* context = this->GetOwner();

	context->SetCurrentRound(context->GetCurrentRound() + 1);
}

void MWCommandBattle::MWBSRoundEnd::ResetActionState()
{
	UMWCommandBattle* context = this->GetOwner();

	auto& actionStates = context->GetActionStates();

	for (auto& state : actionStates)
	{
		state.bActed = false;
	}
}

void MWCommandBattle::MWBSTurnBegin::OnEnter()
{
#if WITH_EDITOR
	DHEndPIE = FEditorDelegates::EndPIE.AddRaw(this, &MWCommandBattle::MWBSTurnBegin::OnEndPIE);
#endif

	UMWCommandBattle* context = this->GetOwner();

	//if (!context->GetBattleSystem().OnActionComplete.IsBoundToObject(this))
	//{
	//	DHActionComplete = context->GetBattleSystem().OnActionComplete.AddRaw(this, &MWCommandBattle::MWBSTurnBegin::OnActionComplete);
	//}

	const EMWTeamAlign currAlign = context->GetCurrentTurnTeamAlign();

	//if (context->GetBattleSystem().OnTurnBegin.IsBound())
	//{
	//	context->GetBattleSystem().OnTurnBegin.Broadcast(currAlign);
	//}

	// Change to battle camera
	SetCharacterCameraAsMain();

	// Create Turn Action - TStrongObjectPtr automatically prevents GC
	switch (currAlign)
	{
		case EMWTeamAlign::Player:
			ActionExecutor = TStrongObjectPtr<UMWActionExecutor>(NewObject<UMWPlayerActionExecutor>(context));
			break;
		case EMWTeamAlign::Enemy:
			ActionExecutor = TStrongObjectPtr<UMWActionExecutor>(NewObject<UMWEnemyActionExecutor>(context));
			break;
	}

	if (ActionExecutor.IsValid())
	{
		ActionExecutor->Init();
		ActionExecutor->SetActionUnits(context->GetPlayerTeam(), context->GetEnemyTeam());
	}
}

void MWCommandBattle::MWBSTurnBegin::OnUpdate(float DeltaTime)
{
	FMWActionExecutorData data;

	if (!bIsActionComplete)
	{
		if (ActionExecutor.IsValid())
		{
			ActionExecutor->Update(DeltaTime, data, bIsActionComplete);
		}
	}

	if (bIsActionComplete)
	{
		UMWCommandBattle* context = this->GetOwner();

		context->GetCurrentTurnActionState().MarkAsActed();

		this->ChangeState(TEXT("MWBSTurnEnd"));
	}
}

void MWCommandBattle::MWBSTurnBegin::OnLeave(bool bShutDown)
{
	bIsActionComplete = false;

	CleanUp();
}

void MWCommandBattle::MWBSTurnBegin::SetCharacterCameraAsMain()
{
	UMWCommandBattle* context = this->GetOwner();

	const FMWTeam& team = context->GetCurrentTurnTeamAlign() == EMWTeamAlign::Player ? context->GetPlayerTeam() : context->GetEnemyTeam();

	auto* actor = team.BattleUnits[0].Get();

	if (actor == nullptr)
	{
		return;
	}

	if (auto* pc = UGameplayStatics::GetPlayerController(context->GetWorld(), 0))
	{
		if (auto* camComp = actor->FindComponentByClass<UC3DCameraComponent>())
		{
			camComp->SetCameraMode(FGameplayTag::RequestGameplayTag("Camera.Mode.Battle"), true);
		}

		pc->SetViewTargetWithBlend(actor);
	}
}

void MWCommandBattle::MWBSTurnBegin::OnDestroy()
{
	CleanUp();
}

void MWCommandBattle::MWBSTurnBegin::OnActionComplete()
{
	bIsActionComplete = true;
}

void MWCommandBattle::MWBSTurnBegin::CleanUp()
{
	if (ActionExecutor.IsValid())
	{
		ActionExecutor->Uninit();
		ActionExecutor.Reset();
	}

	// Clean up delegate binding if not already done
	UMWCommandBattle* context = this->GetOwner();
	if (context && DHActionComplete.IsValid())
	{
		DHActionComplete.Reset();
	}

#if WITH_EDITOR
	if (DHEndPIE.IsValid())
	{
		FEditorDelegates::EndPIE.Remove(DHEndPIE);
		DHEndPIE.Reset();
	}
#endif
}

#if WITH_EDITOR
void MWCommandBattle::MWBSTurnBegin::OnEndPIE(bool bIsSimulating)
{
	CleanUp();
}
#endif

void MWCommandBattle::MWBSTurnEnd::OnEnter()
{
	UMWCommandBattle* context = this->GetOwner();

	//if (context->GetBattleSystem().OnTurnEnd.IsBound())
	//{
	//	context->GetBattleSystem().OnTurnEnd.Broadcast(context->GetCurrentTurnTeamAlign());
	//}

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

void MWCommandBattle::MWBSTurnEnd::OnUpdate(float DeltaTime)
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

void MWCommandBattle::MWBSTurnEnd::CheckShouldEndBattle()
{
	UMWCommandBattle* context = this->GetOwner();

	const FMWTeam& playerTeam = context->GetPlayerTeam();

	const FMWTeam& enemyTeam = context->GetEnemyTeam();

	// check is game end
	if (enemyTeam.IsAlive() && playerTeam.IsAlive())
	{
		return;
	}

	if (!enemyTeam.IsAlive() && !playerTeam.IsAlive())
	{
		context->SetBattleResult(EBattleResult::Draw);
	}

	if (!enemyTeam.IsAlive())
	{
		context->SetBattleResult(EBattleResult::PlayerWin);
	}

	if(!playerTeam.IsAlive())
	{
		context->SetBattleResult(EBattleResult::EnemyWin);
	}

	bIsBattleOver = true;
}

void MWCommandBattle::MWBSTurnEnd::CheckShouldRoundEnd()
{
	UMWCommandBattle* context = this->GetOwner();

	bIsRoundEnd = true;

	auto& actionStates = context->GetActionStates();

	for (auto& state : actionStates)
	{
		bIsRoundEnd &= state.bActed;
	}
}

UMWCommandBattle::UMWCommandBattle()
{
	// Init state machine
	Fsm = MakeShared<FFsm<UMWCommandBattle>>(
		TEXT("BattleFsm"),
		this,
		new MWCommandBattle::MWBSIdle,
		new MWCommandBattle::MWBSBattleBegin,
		new MWCommandBattle::MWBSBattleEnd,
		new MWCommandBattle::MWBSRoundBegin,
		new MWCommandBattle::MWBSRoundEnd,
		new MWCommandBattle::MWBSTurnBegin,
		new MWCommandBattle::MWBSTurnEnd);
}

void UMWCommandBattle::StartBattle(const FMWBattleData& InData)
{
	check(GetWorld () != nullptr);

	// Init battle
	const uint32 teamNo = InData.Teams.Num();

	for (uint32 i = 0; i < teamNo; ++i)
	{
		if (InData.Teams[i].GetTeamAlign() == EMWTeamAlign::Player)
		{
			PlayerTeam = InData.Teams[i];
		}
		else if (InData.Teams[i].GetTeamAlign() == EMWTeamAlign::Enemy)
		{
			EnemyTeam = InData.Teams[i];
		}
	}

	check(PlayerTeam.IsValid() && EnemyTeam.IsValid());

	// Input
	PC = Cast<AMWPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (PC.IsValid())
	{
		const UMWMasterData* data = MWSINGLETON->GetMasterData();
		UMWInputUtility::DisableMappingContext(PC.Get(), data->FindInputConfig(MWGameplayTags::IMC_TPDefault));
	}
	

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

void UMWCommandBattle::Tick(float DeltaTime)
{
	if (GetWorld())
	{
		if (Fsm.IsValid())
		{
			auto* currState = Fsm->GetCurrentState();

			if (!currState)
			{
				return;
			}

			// Force end battle
			// 強制的にバトルを終了させる
			if (bIsBattleEnd && currState->GetName() != TEXT("MWBSBattleEnd"))
			{
				Fsm->ChangeState(TEXT("MWBSBattleEnd"));

				return;
			}

			Fsm->Update(DeltaTime);
			GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Yellow, FString::Printf(TEXT("state[%s,%f] - round : %d"), *currState->GetName().ToString(), Fsm->GetCurrentStateTime(), CurrRound));
		}
	}
}

TStatId UMWCommandBattle::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UMWCommandBattle, STATGROUP_Tickables);
}

bool UMWCommandBattle::IsTickable() const
{
	return bIsTickable;
}

void UMWCommandBattle::BeginDestroy()
{
	if(Fsm.IsValid())
	{
		Fsm->ShutDown();
		Fsm.Reset();
	}

	Super::BeginDestroy();
}

void UMWCommandBattle::SetActionBuffPool(const TArray<EMWBattleActionBuff>& NewBuffPool)
{
	ActionBuffPool = NewBuffPool;
}

void UMWCommandBattle::InitBattleUnits(const FMWBattleData& InData)
{

}

FMWActionState& UMWCommandBattle::GetCurrentTurnActionState()
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

void UMWCommandBattle::EndBattle(EBattleResult InWinner)
{
	bIsBattleEnd = true;
	BattleResult = InWinner;
}

void UMWCommandBattle::Initialize()
{
}

void UMWCommandBattle::Uninitialize()
{
}

UMWBattleSystem& UMWCommandBattle::GetBattleSystem()
{
	return  *UMWBattleSystem::Get(this);
}
UE_ENABLE_OPTIMIZATION