#pragma once

#include "Engine/DataTable.h"
#include "MWCharacterTable.generated.h"

class UMWCharacterAsset;

/**
 * Character row data loaded from DataTable.
 *
 * Rule:
 * - Character static/basic data is stored in this table.
 * - Character detailed asset data is stored in UMWCharacterAsset (PrimaryData).
 * - This row keeps a soft reference to the PrimaryData asset.
 */
USTRUCT(BlueprintType)
struct FMWCharacterTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	FMWCharacterTable();

	/** Character unique id (usually from Excel). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character")
	int32 Id = INDEX_NONE;

	/** Internal name key. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character")
	FName Name = NAME_None;

	/** Localized display name. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character")
	FText DisplayName = FText::GetEmpty();

	/** Soft reference to character primary asset data. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character")
	TSoftObjectPtr<UMWCharacterAsset> Asset;

	/** Minimal row validity check. */
	bool IsValidRow() const;
};