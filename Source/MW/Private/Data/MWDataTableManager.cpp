#include "Data/MWDataTableManager.h"
#include "Data/MWDataAsset.h"
#include "Define/MWDefineDataTable.h"
#include "MWGameSingleton.h"

UMWDataTableManager* UMWDataTableManager::Get()
{
	return MWSINGLETON->GetDataTableManager();
}

void UMWDataTableManager::Initialize()
{
	TRACE_CPUPROFILER_EVENT_SCOPE(MWDataTableManager_Initialize);

	if (DATAASSET == nullptr)
	{
		return;
	}

	bInit = true;

	DataTableMap.Empty();

	if (const FMWSoftPath* softPathData = DATAASSET->GetMWSoftPathData(UMWDataAsset::EDataAssetType::DataTable))
	{
		for (const auto& Pair : softPathData->PathMap)
		{
			GetDataTable(Pair.Key);
		}
	}

	CacheDataTables();
}

void UMWDataTableManager::Reload()
{
	Initialize();
}

UDataTable* UMWDataTableManager::GetDataTable(const FName& TableName)
{
	if (DATAASSET == nullptr)
	{
		return nullptr;
	}

	if (bInit == false)
	{
		return nullptr;
	}

	if (UDataTable** FoundDataTable = DataTableMap.Find(TableName))
	{
		return *FoundDataTable;
	}

	FSoftObjectPath* DataTablePath = DATAASSET->GetObjectPath(UMWDataAsset::EDataAssetType::DataTable, TableName);
	if (DataTablePath)
	{
		UDataTable* DataTable = Cast<UDataTable>(DataTablePath->TryLoad());
		DataTableMap.Add(TableName, DataTable);
		return DataTable;
	}

	return nullptr;
}

UDataTable* UMWDataTableManager::GetDataTable(const FString& TableName)
{
	check(TableName.IsEmpty() == false);
	FName SearchKey = FName(*TableName);
	return GetDataTable(SearchKey);
}

const FMWCharacterTempleteData* UMWDataTableManager::GetCharacterDataById(int32 Id)
{
	auto* data = CharacterDataMap.Find(Id);

	return data ? *data : nullptr;
}

void UMWDataTableManager::CacheDataTables()
{
	GenerateCharacterDataMap();
}

void UMWDataTableManager::GenerateCharacterDataMap()
{
	CharacterDataMap.Empty();

	UDataTable* characterDataTable = GetDataTable(MWDataTableName::CharacterDataTable);

	if (!characterDataTable)
	{
		return;
	}

	TArray<FMWCharacterTempleteData*> tableRows;
	characterDataTable->GetAllRows<FMWCharacterTempleteData>(FMWCharacterTempleteData::StaticStruct()->GetName(), tableRows);

	for (const FMWCharacterTempleteData* data : tableRows)
	{
		CharacterDataMap.FindOrAdd(data->Id) = data;
	}
}
