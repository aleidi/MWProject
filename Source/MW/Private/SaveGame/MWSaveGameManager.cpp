#include "SaveGame/MWSaveGameManager.h"

#define GET_SAVESLOT_NAME(SlotIndex) FString::Printf(TEXT("MW%d"), SlotIndex)

void UMWSaveGameManager::Initialize()
{
	PlayerSaveGame = NewObject<UMWPlayerSaveGame>(this);
}

void UMWSaveGameManager::Deinitialize()
{
	PlayerSaveGame = nullptr;
}

UMWSaveGameManager* UMWSaveGameManager::Get(const UObject* WorldContext)
{
	if (UMWGameInstanceSubsystem* subsystem = UMWGameInstanceSubsystem::Get(WorldContext))
	{
		return subsystem->GetSaveGameManager();
	}

	return nullptr;
}

void UMWSaveGameManager::SaveGameToSlot(int32 SlotIndex)
{
	const FString slotName = GET_SAVESLOT_NAME(SlotIndex);

	if(UGameplayStatics::SaveGameToSlot(PlayerSaveGame, slotName, 0))
	{
		MW_LOG_DEFAULT(TEXT("Game saved successfully to slot: %s"), *slotName);
	}
	else
	{
		MW_LOG_ERROR(TEXT("Failed to save game to slot: %s"), *slotName);
	}
}

void UMWSaveGameManager::LoadGameFromSlot(int32 SlotIndex)
{
	const FString slotName = GET_SAVESLOT_NAME(SlotIndex);

	if (!UGameplayStatics::DoesSaveGameExist(slotName, 0))
	{
		MW_LOG_ERROR(TEXT("No save game exists in slot: %s"), *slotName);

		return;
	}

	if(USaveGame* loadedSave = UGameplayStatics::LoadGameFromSlot(slotName, 0))
	{
		PlayerSaveGame = Cast<UMWPlayerSaveGame>(loadedSave);

		if (PlayerSaveGame)
		{
			MW_LOG_DEFAULT(TEXT("Game loaded successfully from slot: %s"), *slotName);
		}
		else
		{
			MW_LOG_ERROR(TEXT("Failed to cast loaded save game from slot: %s"), *slotName);
		}
	}
	else
	{
		MW_LOG_ERROR(TEXT("Failed to load game from slot: %s"), *slotName);
	}
}

bool UMWSaveGameManager::DoesSaveExist(int32 SlotIndex) const
{
	const FString slotName = GET_SAVESLOT_NAME(SlotIndex);

	return UGameplayStatics::DoesSaveGameExist(slotName, 0);
}

bool UMWSaveGameManager::DeleteSaveSlot(int32 SlotIndex)
{
	const FString slotName = GET_SAVESLOT_NAME(SlotIndex);

	return UGameplayStatics::DeleteGameInSlot(slotName, 0);
}

UMWPlayerSaveGame* UMWSaveGameManager::GetPlayerSaveGame() const
{
	return PlayerSaveGame;
}
