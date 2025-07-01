#pragma once

// Include Header
#include "UI/MWUserWidget.h"
#include "MWCharacterBaseInfo.generated.h"

// Forward Declare
class UImage;
class UCommonTextBlock;
class UProgressBar;

// Macro

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
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> IMG_HeadIcon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> IMG_ActionIcon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> CTB_Level;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> CTB_Name;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> CTB_HP;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> PB_HPBar;
};