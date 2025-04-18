#include "Gameplay/Battle/MWBattleSystem.h"
#include "Gameplay/Battle/MWBattle.h"

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
		BattleInst = nullptr;
	}
	BattleInst = NewObject<UMWBattle>(this);
	BattleInst->StartBattle(InTeams);
	BattleInst->AddToRoot();

	if (DHBattleEnd.IsValid())
	{
		OnBattleEnd.Remove(DHBattleEnd);
	}
	DHBattleEnd = OnBattleEnd.AddUObject(this, &UMWBattleSystem::OnBattleEndCallback);
}

void UMWBattleSystem::EndBattle(uint8 Winner)
{
	if (IsValid(BattleInst))
	{
		BattleInst->ForceEndBattle(Winner);
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
