#include "Data/MWDataTableManager.h"
#include "Data/MWAssetRegistry.h"
#include "Define/MWDefineDataTable.h"
#include "MWGameSingleton.h"

UMWDataTableManager* UMWDataTableManager::Get()
{
	return MWSINGLETON->GetDataTableManager();
}

void UMWDataTableManager::Initialize()
{
	TRACE_CPUPROFILER_EVENT_SCOPE(MWDataTableManager_Initialize);

	if (ASSETREGISTRY() == nullptr)
	{
		return;
	}

	bInit = true;

	DataTableMap.Empty();

	if (const FMWSoftPath* softPathData = ASSETREGISTRY()->GetMWSoftPathData(UMWAssetRegistry::EDataAssetType::DataTable))
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
	if (ASSETREGISTRY() == nullptr)
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

	FSoftObjectPath* DataTablePath = ASSETREGISTRY()->GetObjectPath(UMWAssetRegistry::EDataAssetType::DataTable, TableName);
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

const FMWCharacterTemplateData* UMWDataTableManager::GetCharacterDataById(int32 Id)
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

	TArray<FMWCharacterTemplateData*> tableRows;
	characterDataTable->GetAllRows<FMWCharacterTemplateData>(FMWCharacterTemplateData::StaticStruct()->GetName(), tableRows);

	for (const FMWCharacterTemplateData* data : tableRows)
	{
		CharacterDataMap.FindOrAdd(data->Id) = data;
	}
}
