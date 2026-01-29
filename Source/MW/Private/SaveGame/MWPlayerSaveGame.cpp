#include "SaveGame/MWPlayerSaveGame.h"

void UMWPlayerSaveGame::ChangeData()
{
	// Clear existing data
	PartyData.ActiveMembers.Empty();
	PartyData.ReserveMembers.Empty();

	// Generate random active members (3-4 members)
	int32 ActiveMemberCount = FMath::RandRange(3, 4);
	for (int32 i = 0; i < ActiveMemberCount; ++i)
	{
		FMWPartySlot Slot;
		Slot.CharacterId = FMath::RandRange(1000, 9999); // Random character ID
		Slot.SlotIndex = i;
		Slot.bIsActive = true;
		PartyData.ActiveMembers.Add(Slot);
	}

	// Generate random reserve members (2-5 members)
	int32 ReserveMemberCount = FMath::RandRange(2, 5);
	for (int32 i = 0; i < ReserveMemberCount; ++i)
	{
		FMWPartySlot Slot;
		Slot.CharacterId = FMath::RandRange(1000, 9999); // Random character ID
		Slot.SlotIndex = i;
		Slot.bIsActive = false;
		PartyData.ReserveMembers.Add(Slot);
	}
}
