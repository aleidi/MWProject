#pragma once

#include "Data/Skill/MWSkillAsset.h"
#include "Engine/DataTable.h"
#include "MWSkillTable.generated.h"

class UMWSkillAsset;

USTRUCT(BlueprintType)
struct FMWSkillChargeConfig
{
	GENERATED_BODY()

	/** Maximum charge value. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Charge", meta = (ClampMin = "0.1"))
	float MaxValue = 100.0f;

	/** Time (seconds) required to reach full charge. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Charge", meta = (ClampMin = "0.01"))
	float FullTime = 1.0f;

	/** Charge decay speed after release. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Charge", meta = (ClampMin = "0.1"))
	float DischargeRate = 90.0f;

	/** Tap window before charge starts. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Charge", meta = (ClampMin = "0.0"))
	float StartDelay = 0.15f;

	/** Runtime charge gain per second. */
	float GetChargeRate() const
	{
		return MaxValue / FMath::Max(FullTime, 0.01f);
	}
};

UENUM(BlueprintType)
enum class EMWSkillNoTargetPolicy : uint8
{
	/** Cancel cast when no valid target is found. */
	FailCast	UMETA(DisplayName = "Fail Cast"),

	/** Force close form when no valid target is found. */
	UseClose	UMETA(DisplayName = "Use Close"),

	/** Force far form when no valid target is found. */
	UseFar		UMETA(DisplayName = "Use Far"),
};

USTRUCT(BlueprintType)
struct FMWSkillRangeAdaptiveConfig
{
	GENERATED_BODY()

	/** Enter close form when distance <= this value. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RangeAdaptive", meta = (ClampMin = "0.0"))
	float NearEnterDistance = 350.0f;

	/** Exit close form when distance >= this value. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RangeAdaptive", meta = (ClampMin = "0.0"))
	float NearExitDistance = 450.0f;

	/** Search radius used when resolving a fallback target. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RangeAdaptive", meta = (ClampMin = "0.0"))
	float TargetSearchRadius = 2000.0f;

	/** Behavior when no valid target is available. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RangeAdaptive")
	EMWSkillNoTargetPolicy NoTargetPolicy = EMWSkillNoTargetPolicy::FailCast;

	/** Montage section used by close form. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RangeAdaptive")
	FName CloseSectionName = TEXT("Close");

	/** Montage section used by far form. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RangeAdaptive")
	FName FarSectionName = TEXT("Far");
};

USTRUCT(BlueprintType)
struct FMWSkillStockConfig
{
	GENERATED_BODY()

	/** Max usable charges for this skill. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stock", meta = (ClampMin = "1"))
	int32 MaxUses = 3;

	/** Recover points gained per second. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stock", meta = (ClampMin = "0.0"))
	float RecoverAmount = 100.0f;

	/** Points needed to recover one use. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stock", meta = (ClampMin = "1.0"))
	float RecoverPointThreshold = 100.0f;

	/** Delay (seconds) after consume before recovery starts. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stock", meta = (ClampMin = "0.0"))
	float RecoverDelayAfterConsume = 0.0f;
};

USTRUCT(BlueprintType)
struct FMWSkillTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	FMWSkillTable();

	/** Unique skill id. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, AssetRegistrySearchable)
	int32 Id = INDEX_NONE;

	/** Internal row name. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName Name = NAME_None;

	/** Localized display name. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText DisplayName = FText::GetEmpty();

	/** Localized description text. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (MultiLine = "true"))
	FText Description = FText::GetEmpty();

	/** Skill type classification. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Classification")
	EMWSkillType Type = EMWSkillType::Active;

	/** Optional business tag used for filtering/grouping. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Classification")
	FGameplayTag SkillTag;

	/** Slot cost in loadout. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loadout", meta = (ClampMin = "1"))
	int32 SlotCost = 1;

	/** Maximum upgrade level. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Progression", meta = (ClampMin = "1"))
	int32 MaxLevel = 1;

	/** Soft reference to skill primary asset data. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	TSoftObjectPtr<UMWSkillAsset> Asset;

	/** Charge tuning config. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Charge")
	FMWSkillChargeConfig ChargeConfig;

	/** Whether this skill uses range-adaptive close/far forms. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RangeAdaptive")
	bool bIsRangeAdaptive = false;

	/** Hidden when bIsRangeAdaptive is false. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RangeAdaptive", meta = (EditCondition = "bIsRangeAdaptive", EditConditionHides))
	FMWSkillRangeAdaptiveConfig RangeAdaptiveConfig;

	/** Stock configuration for the skill. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stock")
	FMWSkillStockConfig StockConfig;

	/** Minimal row validity check. */
	bool IsValidRow() const;
};
