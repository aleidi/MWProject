#include "SaveGame/MWSaveGameManager.h"

#include "Kismet/GameplayStatics.h"
#include "MWLogChannels.h"
#include "SaveGame/MWSaveGame.h"
#include "System/MWGameInstanceSubsystem.h"

#define GET_SAVESLOTNAME(SlotIndex) FString::Printf(TEXT("MW%d"), SlotIndex)

void UMWSaveGameManager::Initialize()
{
	SaveGameInstance = NewObject<UMWSaveGame>(this);
}

void UMWSaveGameManager::Deinitialize()
{
	SaveGameInstance = nullptr;
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
	const FString slotName = GET_SAVESLOTNAME(SlotIndex);

	if(UGameplayStatics::SaveGameToSlot(SaveGameInstance, slotName, 0))
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
	const FString slotName = GET_SAVESLOTNAME(SlotIndex);

	if (!UGameplayStatics::DoesSaveGameExist(slotName, 0))
	{
		MW_LOG_ERROR(TEXT("No save game exists in slot: %s"), *slotName);

		return;
	}

	if(USaveGame* loadedSave = UGameplayStatics::LoadGameFromSlot(slotName, 0))
	{
		SaveGameInstance = Cast<UMWSaveGame>(loadedSave);

		if (SaveGameInstance)
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
	const FString slotName = GET_SAVESLOTNAME(SlotIndex);

	return UGameplayStatics::DoesSaveGameExist(slotName, 0);
}

bool UMWSaveGameManager::DeleteSaveSlot(int32 SlotIndex)
{
	const FString slotName = GET_SAVESLOTNAME(SlotIndex);

	return UGameplayStatics::DeleteGameInSlot(slotName, 0);
}

UMWSaveGame* UMWSaveGameManager::GetSaveGame() const
{
	return SaveGameInstance;
}
