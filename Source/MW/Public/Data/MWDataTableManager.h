#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "MWDataTableManager.generated.h"

namespace MWDataTableName
{
	static const FString CharacterDataTable = TEXT("DT_Character");
}

class UDataTable;
struct FMWCharacterTemplateData;
struct FTableRowBase;

#define DATATABLEMANAGER() (UMWDataTableManager::Get())

/*
 * @class UMWDataTableManager
 *
 * @brief Quick access to data tables.
 *       　データテーブルへのクイックアクセス。
 *
 */
UCLASS()
class MW_API UMWDataTableManager : public UObject
{
	GENERATED_BODY()

public:
	static UMWDataTableManager* Get();
	void Initialize();
	void Reload();

	UDataTable* GetDataTable(const FString& TableName);
	UDataTable* GetDataTable(const FName& TableName);

	template <typename T UE_REQUIRES(TIsDerivedFrom<T, FTableRowBase>::IsDerived)>
	void GetDataTableAllRows(const FString& TableName, TArray<T*>& OutRowArray)
	{
		if (UDataTable* DataTable = GetDataTable(TableName))
		{
			DataTable->GetAllRows(FString(), OutRowArray);
		}
	}

	template <typename T UE_REQUIRES(TIsDerivedFrom<T, FTableRowBase>::IsDerived)>
	T* GetDataTableItem(const FString& TableName, FName RowName)
	{
		if (UDataTable* DataTable = GetDataTable(TableName))
		{
			return DataTable->FindRow<T>(RowName, FString());
		}

		return nullptr;
	}

	template <typename T UE_REQUIRES(TIsDerivedFrom<T, FTableRowBase>::IsDerived)>
	T* GetDataTableItem(const FString& TableName, int32 Index)
	{
		if (UDataTable* DataTable = GetDataTable(TableName))
		{
			return DataTable->FindRow<T>(FName(*FString::FromInt(Index)), FString());
		}

		return nullptr;
	}

	const FMWCharacterTemplateData* GetCharacterDataById(int32 Id);

private:
	void CacheDataTables();

	void GenerateCharacterDataMap();

private:
	UPROPERTY(Transient)
	TMap<FName, UDataTable*> DataTableMap;

	UPROPERTY(Transient)
	bool bInit = false;

	TMap<int32, const FMWCharacterTemplateData*> CharacterDataMap;
};