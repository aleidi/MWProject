#include "Gameplay/MWCheatManager.h"
#include "Define/MWStruct.h"
#include "Gameplay/Battle/MWBattleSystem.h"

void UMWCheatManager::TargetSelctedTest(const FMWFoundActorInfo& TargetInfo)
{
	if (UMWBattleSystem* mwbs = GetWorld()->GetSubsystem<UMWBattleSystem>())
	{
		if (mwbs->OnTargetSelected.IsBound())
		{
			mwbs->OnTargetSelected.Broadcast(TargetInfo);
		}
	}
}
