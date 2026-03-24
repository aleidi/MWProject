#include "SaveGame/MWSaveGame.h"

void UMWSaveGame::ChangeData()
{
	// Clear existing data
	PartyData.PartyMembers.Empty();

	// Generate random active members (3-4 members)
	int32 ActiveMemberCount = FMath::RandRange(3, 4);
	for (int32 i = 0; i < ActiveMemberCount; ++i)
	{
		FMWCharacterSaveData data;
		data.CharacterId = FMath::RandRange(1000, 9999); // Random character ID
		data.Level = i;
		PartyData.PartyMembers.Add(data);
	}
}
