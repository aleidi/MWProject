#include "Gameplay/Battle/MWBattleSystem.h"
#include "Gameplay/Battle/MWCommandBattle.h"
#include "GameFramework/GameModeBase.h"
#include "System/MWGameInstanceSubsystem.h"

UMWBattleSystem* UMWBattleSystem::Get(const UObject* WorldContext)
{
	if (UMWGameInstanceSubsystem* subsystem = UMWGameInstanceSubsystem::Get(WorldContext))
	{
		return subsystem->GetBattleManager();
	}

	return nullptr;
}
