#pragma once

#include "Data/Skill/MWSkillAsset.h"
#include "Engine/DataTable.h"
#include "MWSkillTable.generated.h"

class UMWSkillAsset;

USTRUCT(BlueprintType)
struct FMWSkillChargeConfig
{
	GENERATED_BODY()

	/** 最大チャージ値。 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Charge", meta = (ClampMin = "0.1"))
	float MaxValue = 100.0f;

	/** 最大チャージまでの所要時間（秒）。 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Charge", meta = (ClampMin = "0.01"))
	float FullTime = 1.0f;

	/** 入力を離した後のチャージ減衰速度。 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Charge", meta = (ClampMin = "0.1"))
	float DischargeRate = 90.0f;

	/** チャージ開始前のTap受付時間。 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Charge", meta = (ClampMin = "0.0"))
	float StartDelay = 0.15f;

	/** 実行時の1秒あたりのチャージ増加量。 */
	float GetChargeRate() const
	{
		return MaxValue / FMath::Max(FullTime, 0.01f);
	}
};

UENUM(BlueprintType)
enum class EMWSkillNoTargetPolicy : uint8
{
	/** 有効なターゲットがない場合はキャストを中止します。 */
	FailCast	UMETA(DisplayName = "Fail Cast"),

	/** 有効なターゲットがない場合は近距離形態を使用します。 */
	UseClose	UMETA(DisplayName = "Use Close"),

	/** 有効なターゲットがない場合は遠距離形態を使用します。 */
	UseFar		UMETA(DisplayName = "Use Far"),
};

USTRUCT(BlueprintType)
struct FMWSkillRangeAdaptiveConfig
{
	GENERATED_BODY()

	/** 距離がこの値以下の場合に近距離形態へ移行します。 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RangeAdaptive", meta = (ClampMin = "0.0"))
	float NearEnterDistance = 350.0f;

	/** 距離がこの値以上の場合に近距離形態を解除します。 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RangeAdaptive", meta = (ClampMin = "0.0"))
	float NearExitDistance = 450.0f;

	/** 代替ターゲットの選定に使用する検索半径。 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RangeAdaptive", meta = (ClampMin = "0.0"))
	float TargetSearchRadius = 2000.0f;

	/** 有効なターゲットがない場合の動作。 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RangeAdaptive")
	EMWSkillNoTargetPolicy NoTargetPolicy = EMWSkillNoTargetPolicy::FailCast;

	/** 近距離形態で使用するMontageセクション。 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RangeAdaptive")
	FName CloseSectionName = TEXT("Close");

	/** 遠距離形態で使用するMontageセクション。 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RangeAdaptive")
	FName FarSectionName = TEXT("Far");
};

USTRUCT(BlueprintType)
struct FMWSkillStockConfig
{
	GENERATED_BODY()

	/** このスキルの最大使用回数。 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stock", meta = (ClampMin = "1"))
	int32 MaxUses = 3;

	/** 1秒あたりに獲得する回復ポイント。 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stock", meta = (ClampMin = "0.0"))
	float RecoverAmount = 100.0f;

	/** 使用回数を1回復するために必要なポイント。 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stock", meta = (ClampMin = "1.0"))
	float RecoverPointThreshold = 100.0f;

	/** 消費後、回復開始までの遅延時間（秒）。 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stock", meta = (ClampMin = "0.0"))
	float RecoverDelayAfterConsume = 0.0f;
};

USTRUCT(BlueprintType)
struct FMWSkillTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	FMWSkillTable();

	/** スキル固有ID。 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, AssetRegistrySearchable)
	int32 Id = INDEX_NONE;

	/** 内部行名。 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName Name = NAME_None;

	/** ローカライズ済み表示名。 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText DisplayName = FText::GetEmpty();

	/** ローカライズ済み説明文。 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (MultiLine = "true"))
	FText Description = FText::GetEmpty();

	/** スキル種別。 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Classification")
	EMWSkillType Type = EMWSkillType::Active;

	/** フィルタリングやグループ化に使用する任意の管理タグ。 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Classification")
	FGameplayTag SkillTag;

	/** ロードアウトで消費するスロット数。 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loadout", meta = (ClampMin = "1"))
	int32 SlotCost = 1;

	/** 最大強化レベル。 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Progression", meta = (ClampMin = "1"))
	int32 MaxLevel = 1;

	/** スキルPrimaryAssetDataへのソフト参照。 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	TSoftObjectPtr<UMWSkillAsset> Asset;

	/** チャージ調整設定。 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Charge")
	FMWSkillChargeConfig ChargeConfig;

	/** 距離適応型の近距離／遠距離形態を使用するか。 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RangeAdaptive")
	bool bIsRangeAdaptive = false;

	/** bIsRangeAdaptiveがfalseの場合は非表示になります。 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RangeAdaptive", meta = (EditCondition = "bIsRangeAdaptive", EditConditionHides))
	FMWSkillRangeAdaptiveConfig RangeAdaptiveConfig;

	/** スキルの使用回数設定。 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stock")
	FMWSkillStockConfig StockConfig;

	/** 行データの最低限の妥当性を確認します。 */
	bool IsValidRow() const;
};
