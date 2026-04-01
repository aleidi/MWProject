#include "Data/Character/MWCharacterDataManager.h"

#include "Data/Character/MWCharacterData.h"
#include "Define/MWDefineCommon.h"
#include "Engine/DataTable.h"
#include "MWLogChannels.h"
#include "System/MWGameInstanceSubsystem.h"

FString UMWCharacterDataManager::DataPath = DATA_PATH;
FString UMWCharacterDataManager::CharacterDataPath = CHARACTER_DATA_PATH;

UMWCharacterDataManager::UMWCharacterDataManager()
{
}

void UMWCharacterDataManager::Initialize()
{
	LoadCharacterData();
}

UMWCharacterDataManager* UMWCharacterDataManager::Get(const UObject* WorldContext)
{
	if (UMWGameInstanceSubsystem* subsystem = UMWGameInstanceSubsystem::Get(WorldContext))
	{
		return subsystem->GetCharacterDataManager();
	}

	return nullptr;
}

bool UMWCharacterDataManager::LoadCharacterData()
{
	const FString dataPath = DataPath / CharacterDataPath;

	UDataTable* loadedTable = LoadObject<UDataTable>(nullptr, *dataPath);
	if (!IsValid(loadedTable))
	{
		UE_LOG(LogMWData, Warning, TEXT("UMWCharacterDataManager: LoadObject failed -> %s"), *dataPath);

		return false;
	}

	if (loadedTable->GetRowStruct() != FMWCharacterDataRow::StaticStruct())
	{
		UE_LOG(LogMWData, Warning, TEXT("UMWCharacterDataManager: RowStruct mismatch for %s"), *dataPath);

		return false;
	}

	CharacterData = loadedTable;

	UE_LOG(LogMWData, Log, TEXT("UMWCharacterDataManager: CharacterData loaded -> %s"), *dataPath);

	return true;
}

FMWCharacterDataRow* UMWCharacterDataManager::GetCharacterData(int32 CharacterId) const
{
	const FName rowName = FName(*FString::FromInt(CharacterId));

	return CharacterData->FindRow<FMWCharacterDataRow>(rowName, TEXT("GetCharacterData"));
}

bool UMWCharacterDataManager::K2_GetCharacterData(int32 CharacterId, FMWCharacterDataRow& OutData) const
{
	const FMWCharacterDataRow* row = GetCharacterData(CharacterId);
	if (!row)
	{
		return false;
	}

	OutData = *row;

	return true;
}
