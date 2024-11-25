#include "Gameplay/MWCheatManager.h"
#include "Gameplay/MWGameplayTypes.h"
#include "Subsystem/MWBattleSystem.h"

void UMWCheatManager::TargetSelctedTest(const FMWActorInfo& TargetInfo)
{
	if (UMWBattleSystem* mwbs = GetWorld()->GetSubsystem<UMWBattleSystem>())
	{
		if (mwbs->OnTargetSelected.IsBound())
		{
			mwbs->OnTargetSelected.Broadcast(TargetInfo);
		}
	}
}
