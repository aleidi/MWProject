#pragma once

// ヘッダーをインクルード
#include "UI/MWUserWidget.h"
#include "MWCombatSkillInfo.generated.h"

// 前方宣言
class UCommonTextBlock;
class UImage;

// 定義

/*
 * @class UMWCombatSkillInfo
 * 
 * @brief
 *
 * @note
 */
UCLASS()
class UMWCombatSkillInfo : public UMWUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> CTB_SkillName;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> IMG_SkillIcon;
};
