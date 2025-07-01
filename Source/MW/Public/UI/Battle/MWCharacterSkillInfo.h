#pragma once

// Include Header
#include "UI/MWUserWidget.h"
#include "MWCharacterSkillInfo.generated.h"

// Forward Declare
class UCommonTextBlock;
class UImage;

// Macro

/*
 * @class UMWCharacterSkillInfo
 * 
 * @brief
 *
 * @note
 */
UCLASS()
class UMWCharacterSkillInfo : public UMWUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> CTB_SkillName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> IMG_SkillIcon;
};