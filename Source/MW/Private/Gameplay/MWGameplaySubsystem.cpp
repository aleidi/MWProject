#include "Gameplay/MWGameplaySubsystem.h"
#include "Gameplay/MWGameplayTypes.h"

UDataTable* UMWGameplaySubsystem::GetTableCharacterInfo() const
{
	FString path = DT_CHARACTERINFO;
	UDataTable* dt = LoadObject<UDataTable>(nullptr, *path);
	return dt;
}
 