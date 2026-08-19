#pragma once

// ヘッダーをインクルード
#include "UI/MWUserWidget.h"
#include "MWCharacterBaseInfo.generated.h"

// 前方宣言
class UImage;
class UCommonTextBlock;
class UProgressBar;

// 定義

/*
 * @class UMWCharacterBaseInfo
 * 
 * @brief
 *
 * @note
 */
UCLASS()
class UMWCharacterBaseInfo : public UMWUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> IMG_HeadIcon;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> IMG_ActionIcon;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> CTB_Level;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> CTB_Name;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> CTB_HP;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UProgressBar> PB_HPBar;
};
