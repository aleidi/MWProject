#pragma once

// ヘッダーをインクルード
#include "Animation/Notify/MWAnimNotify.h"
#include "GameplayTagContainer.h"
#include "AN_DamageCheck.generated.h"

// 前方宣言

// 定義

/*
 * @class UAN_DamageCheck
 * 
 * @brief スキルアニメーションの指定時点でダメージ計算を実行するAnimNotifyです。
 *
 * @note チャージスキルで事前に取得したターゲットへダメージを適用します。
 */
UCLASS()
class UAN_DamageCheck : public UMWAnimNotify
{
	GENERATED_BODY()
	
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	virtual FString GetNotifyName_Implementation() const override;

protected:
	/** ダメージイベント識別タグ（例："Damage.Skill.Slash.First"）。 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage", meta = (Categories = "Damage"))
	FGameplayTag DamageEventTag;

	/** このヒットタイミングに適用するダメージ倍率。 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float DamageMultiplier = 1.0f;

	/** 多段コンボ内のヒット番号（0：初段、1：2段目など）。 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage", meta = (ClampMin = "0", UIMin = "0"))
	int32 HitIndex = 0;
};