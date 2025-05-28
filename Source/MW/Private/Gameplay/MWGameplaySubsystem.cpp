#include "Gameplay/MWGameplaySubsystem.h"

#define DT_CHARACTERINFO TEXT("/Game/Datatable/Character/DT_CharacterInfo.DT_CharacterInfo")

UDataTable* UMWGameplaySubsystem::GetTableCharacterInfo() const
{
	FString path = DT_CHARACTERINFO;
	UDataTable* dt = LoadObject<UDataTable>(nullptr, *path);
	return dt;
}
 