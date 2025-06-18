#include "Gameplay/Battle/MWBattleSystem.h"
#include "Gameplay/Battle/MWBattle.h"
#include "GameFramework/GameModeBase.h"

void UMWBattleSystem::ActionComplete()
{
	if (OnActionComplete.IsBound())
	{
		OnActionComplete.Broadcast();
	}
}

void UMWBattleSystem::StartBattle(const FMWBattleData& InData)
{
	if (IsValid(BattleInst))
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("There is a already battle is running"));
		return;
	}

	// Init battle instance
	BattleInst = NewObject<UMWBattle>(this);
	BattleInst->StartBattle(InData);
	BattleInst->AddToRoot();

	if (DHBattleEnd.IsValid())
	{
		OnBattleEnd.Remove(DHBattleEnd);
	}
	DHBattleEnd = OnBattleEnd.AddUObject(this, &UMWBattleSystem::OnBattleEndCallback);

	FGameModeEvents::OnGameModeLogoutEvent().AddUObject(this, &UMWBattleSystem::OnGameModeLogout);
}

void UMWBattleSystem::EndBattle(uint8 Winner)
{
	if (IsValid(BattleInst))
	{
		BattleInst->ForceEndBattle((EBattleResult)Winner);
	}
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
