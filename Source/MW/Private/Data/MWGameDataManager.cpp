#include "Data/MWGameDataManager.h"

#include "Data/MWGameDataTypes.h"
#include "Define/MWDefineCommon.h"
#include "Engine/DataTable.h"
#include "MWLogChannels.h"
#include "System/MWGameInstanceSubsystem.h"

UMWGameDataManager::UMWGameDataManager()
{
	DataPath = DATA_PATH;
	CharacterDataPath = CHARACTER_DATA_PATH;
}

void UMWGameDataManager::Initialize()
{
	LoadAllData();
}

UMWGameDataManager* UMWGameDataManager::Get(const UObject* WorldContext)
{
	if (UMWGameInstanceSubsystem* subsystem = UMWGameInstanceSubsystem::Get(WorldContext))
	{
		return subsystem->GetGameDataManager();
	}

	return nullptr;
}

void UMWGameDataManager::LoadAllData()
{
	if (!LoadCharacterData())
	{
		UE_LOG(LogTemp, Error, TEXT("UMWGameDataManager: Failed to load character data table"));
	}
}

bool UMWGameDataManager::LoadCharacterData()
{
	const FString dataPath = DataPath / CharacterDataPath;

	UDataTable* loadedTable = LoadObject<UDataTable>(nullptr, *dataPath);
	if (!IsValid(loadedTable))
	{
		UE_LOG(LogMWData, Warning, TEXT("UMWGameDataManager: LoadObject failed -> %s"), *dataPath);

		return false;
	}

	if (loadedTable->GetRowStruct() != FMWCharacterRegistryRow::StaticStruct())
	{
		UE_LOG(LogMWData, Warning, TEXT("UMWGameDataManager: RowStruct mismatch for %s"), *dataPath);

		return false;
	}

	CharacterData = loadedTable;

	UE_LOG(LogMWData, Log, TEXT("UMWGameDataManager: CharacterData loaded -> %s"), *dataPath);

	return true;
}

FMWCharacterRegistryRow* UMWGameDataManager::GetCharacterData(int32 CharacterId) const
{
	const FName rowName = FName(*FString::FromInt(CharacterId));

	return CharacterData->FindRow<FMWCharacterRegistryRow>(rowName, TEXT("GetCharacterData"));
}

bool UMWGameDataManager::K2_GetCharacterData(int32 CharacterId, FMWCharacterRegistryRow& OutData) const
{
	const FMWCharacterRegistryRow* row = GetCharacterData(CharacterId);
	if (!row)
	{
		return false;
	}

	OutData = *row;

	return true;
}
