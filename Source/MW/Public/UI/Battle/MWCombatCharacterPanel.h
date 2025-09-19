#pragma once

// Include Header
#include "UI/MWUserWidget.h"
#include "MWCombatCharacterPanel.generated.h"

// Forward Declare
class UMWCharacterBaseInfo;
class UMWCharacterSkillInfo;
class UUniformGridPanel;

// Define

/*
 * @class UMWCombatCharacterPanel
 * 
 * @brief
 *
 * @note
 */
UCLASS()
class UMWCombatCharacterPanel : public UMWUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UMWCharacterBaseInfo> WBP_MainCharacter;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UMWCharacterBaseInfo> WBP_SubCharacter1;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UMWCharacterBaseInfo> WBP_SubCharacter2;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UMWCharacterSkillInfo> WBP_Skill1;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UMWCharacterSkillInfo> WBP_Skill2;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UMWCharacterSkillInfo> WBP_Skill3;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> UGP_BuffList;
};