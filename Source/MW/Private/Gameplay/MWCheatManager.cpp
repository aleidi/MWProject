#include "Gameplay/MWCheatManager.h"
#include "Define/MWDefineGameplay.h"
#include "Gameplay/Battle/MWBattleSystem.h"

void UMWCheatManager::TargetSelctedTest(const FMWFoundActorInfo& TargetInfo)
{
	if (UMWBattleSystem* mwbs = UMWBattleSystem::Get(this))
	{
		if (mwbs->OnTargetSelected.IsBound())
		{
			mwbs->OnTargetSelected.Broadcast(TargetInfo);
		}
	}
}
