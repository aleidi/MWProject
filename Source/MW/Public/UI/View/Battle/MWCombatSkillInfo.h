#pragma once

// Include Header
#include "UI/MWUserWidget.h"
#include "MWCombatSkillInfo.generated.h"

// Forward Declare
class UCommonTextBlock;
class UImage;

// Define

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
