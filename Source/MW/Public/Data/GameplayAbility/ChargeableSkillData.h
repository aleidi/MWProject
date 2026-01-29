#pragma once 

#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "NiagaraSystem.h"
#include "ChargeableSkillData.generated.h"

class UAnimMontage;
class UNiagaraSystem;

UENUM(BlueprintType)
enum class EMWInputChargeStage : uint8
{
    NoCharge    UMETA(DisplayName="Instant"),
    Light       UMETA(DisplayName="Primed"),
    Perfect     UMETA(DisplayName="Perfect"),
    Overcharge  UMETA(DisplayName="Overcharge"),
	Max			UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FMWInputChargeThresholds
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TapThreshold = 0.15f;

	// Perfect charge window range [PerfectMin, PerfectMax]
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float PerfectMin = 0.8f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float PerfectMax = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxHold = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bAutoReleaseOnMax = true;

	void Sanitize()
	{
		PerfectMin = FMath::Max(TapThreshold, PerfectMin);
		PerfectMax = FMath::Max(PerfectMin, PerfectMax);
		MaxHold = FMath::Max(PerfectMax, MaxHold);
	}

};

USTRUCT(BlueprintType)
struct FMWInputChargeStageSkillAnim
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UAnimMontage> Montage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName MontageSection = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float PlayRate = 1.f;
};

USTRUCT(BlueprintType)
struct FMWChargeStageCue
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag ReleaseCueTag;
};

USTRUCT(BlueprintType)
struct FMWInputChargeVFXConfig
{
	GENERATED_BODY()

	// 按住时的充能环 Niagara（可选）
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Charging")
	TSoftClassPtr<UNiagaraSystem> ChargeLoopSystem;

	// 充能环附着点（如武器 Socket），为空用 Root/Mesh
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Charging")
	FName ChargeLoopAttachSocket = NAME_None;

	// 进入完美窗口时的一次性提示 VFX
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Charging")
	TSoftObjectPtr<UNiagaraSystem> PerfectHintSystem;

	// 释放瞬间按阶段触发的 GameplayCue 映射（由能力在释放时执行）
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Release")
    TMap<EMWInputChargeStage, FMWChargeStageCue> StageReleaseCues;
};

UCLASS(BlueprintType)
class UMWChargeableSkillData : public UDataAsset
{
	GENERATED_BODY()

public:
	/* ===========================
	* 蓄力阈值
	* =========================== */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Charge")
	FMWInputChargeThresholds Thresholds;

	/*========================== =
	* 阶段动画（阶段→Montage / Section / 速率）
	* ========================== = */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Animation")
	TMap<EMWInputChargeStage, FMWInputChargeStageSkillAnim> SkillAnims;

	/* ===========================
	*  VFX 配置（过程性 + 阶段释放 Cue）
	* =========================== */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="VFX")
	FMWInputChargeVFXConfig VFXConfig;
};