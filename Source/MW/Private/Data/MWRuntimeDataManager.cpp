#include "Data/MWRuntimeDataManager.h"
#include "System/MWGameInstanceSubsystem.h"

UMWRuntimeDataManager* UMWRuntimeDataManager::Get(const UObject* WorldContext)
{
	if (UMWGameInstanceSubsystem* Subsystem = UMWGameInstanceSubsystem::Get(WorldContext))
	{
		return Subsystem->GetRuntimeDataManager();
	}

	return nullptr;
}
