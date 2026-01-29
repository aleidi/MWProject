#include "Gameplay/MWPartyManager.h"
#include "System/MWGameInstanceSubsystem.h"

UMWPartyManager* UMWPartyManager::Get(const UObject* WorldContext)
{
	if (UMWGameInstanceSubsystem* subsystem = UMWGameInstanceSubsystem::Get(WorldContext))
	{
		return subsystem->GetPartyManager();
	}

	return nullptr;
}
