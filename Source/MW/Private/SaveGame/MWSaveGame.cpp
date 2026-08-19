#include "SaveGame/MWSaveGame.h"

void UMWSaveGame::ChangeData()
{
	// 既存データをクリア
	PartyData.PartyMembers.Empty();

	// アクティブメンバーを3～4人ランダム生成
	int32 ActiveMemberCount = FMath::RandRange(3, 4);
	for (int32 i = 0; i < ActiveMemberCount; ++i)
	{
		FMWCharacterSaveData data;
		data.CharacterId = FMath::RandRange(1000, 9999); // ランダムなキャラクターID
		data.Level = i;
		PartyData.PartyMembers.Add(data);
	}
}
