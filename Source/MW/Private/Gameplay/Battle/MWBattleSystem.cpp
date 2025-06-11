#include "Gameplay/Battle/MWBattleSystem.h"
#include "Gameplay/Battle/MWBattle.h"
#include "GameFramework/GameModeBase.h"
#include "Gameplay/Battle/MWBattleManager.h"

void UMWBattleSystem::ActionComplete()
{
	if (OnActionComplete.IsBound())
	{
		OnActionComplete.Broadcast();
	}
}

void UMWBattleSystem::StartBattle(const TArray<FMWTeam>& InTeams)
{
	if (IsValid(BattleInst))
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("There is a already battle is running"));
		return;
	}

	// Init battle instance
	BattleInst = NewObject<UMWBattle>(this);
	BattleInst->StartBattle(InTeams);
	BattleInst->AddToRoot();

	if (DHBattleEnd.IsValid())
	{
		OnBattleEnd.Remove(DHBattleEnd);
	}
	DHBattleEnd = OnBattleEnd.AddUObject(this, &UMWBattleSystem::OnBattleEndCallback);

	FGameModeEvents::OnGameModeLogoutEvent().AddUObject(this, &UMWBattleSystem::OnGameModeLogout);

	// Init Battle Manager
	BattleMng = MakeShared<FMWBattleManager>();
}

void UMWBattleSystem::EndBattle(uint8 Winner)
{
	if (IsValid(BattleInst))
	{
		BattleInst->ForceEndBattle(Winner);
	}

	if (BattleMng.IsValid())
	{
		BattleMng.Reset();
	}
}

FMWBattleManager* UMWBattleSystem::GetBattleManager() const
{
	return BattleMng.Get();
}

void UMWBattleSystem::OnBattleEndCallback(EBattleResult Result)
{
	if (IsValid(BattleInst))
	{
		BattleInst->RemoveFromRoot();
		BattleInst = nullptr;
	}

	OnBattleEnd.Remove(DHBattleEnd);
}

void UMWBattleSystem::OnGameModeLogout(AGameModeBase* GameMode, AController* Exiting)
{
	if (GetWorld()->GetFirstPlayerController() == Exiting)
	{
		OnBattleEndCallback(EBattleResult::PlayerWin);
	}
}
