#pragma once

#include "Data/Skill/MWSkillAsset.h"
#include "Engine/DataTable.h"
#include "MWSkillTable.generated.h"

class UMWSkillAsset;

USTRUCT(BlueprintType)
struct FMWSkillChargeConfig
{
	GENERATED_BODY()

	/** Charge max value */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Charge", meta = (ClampMin = "0.1"))
	float MaxValue = 100.0f;

	/** Time (seconds) to reach full charge */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Charge", meta = (ClampMin = "0.01"))
	float FullTime = 1.0f;

	/** Charge decay speed */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Charge", meta = (ClampMin = "0.1"))
	float DischargeRate = 90.0f;

	/** Tap window */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Charge", meta = (ClampMin = "0.0"))
	float StartDelay = 0.15f;

	float GetChargeRate() const
	{
		return MaxValue / FMath::Max(FullTime, 0.01f);
	}
};

/**
 * Skill row data loaded from DataTable.
 *
 * Rule:
 * - Skill lightweight/index data is stored in this table.
 * - Skill detailed asset data is stored in UMWSkillAsset (PrimaryData).
 * - This row keeps a soft reference to the PrimaryData asset.
 */
USTRUCT(BlueprintType)
struct FMWSkillTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	FMWSkillTable();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, AssetRegistrySearchable)
	int32 Id = INDEX_NONE;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName Name = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText DisplayName = FText::GetEmpty();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (MultiLine = "true"))
	FText Description = FText::GetEmpty();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Classification")
	EMWSkillType Type = EMWSkillType::Active;

	/** Business tag for filtering/grouping (optional). */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Classification")
	FGameplayTag SkillTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loadout", meta = (ClampMin = "1"))
	int32 SlotCost = 1;

	/** Max upgrade level. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Progression", meta = (ClampMin = "1"))
	int32 MaxLevel = 1;

	/** Soft reference to skill primary asset data. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	TSoftObjectPtr<UMWSkillAsset> Asset;

	/** Charge tuning config */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Charge")
	FMWSkillChargeConfig ChargeConfig;

	/** Minimal row validity check. */
	bool IsValidRow() const;
};