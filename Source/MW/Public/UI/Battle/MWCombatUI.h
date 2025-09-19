#pragma once

// Include Header
#include "UI/MWUserWidget.h"
#include "MWCombatUI.generated.h"

// Forward Declare
class UVerticalBox;

// Define

/*
 * @class UMWCombatUI
 * 
 * @brief
 *
 * @note
 */
UCLASS()
class UMWCombatUI : public UMWUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> VB_CharacterPanel;

protected:
	virtual void NativePreConstruct() override;

	virtual void NativeConstruct() override;

};