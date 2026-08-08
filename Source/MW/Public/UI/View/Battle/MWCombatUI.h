#pragma once

// Include Header
#include "UI/MWUserWidget.h"
#include "MWCombatUI.generated.h"

// Forward Declare
class UMWCombatSkillBar;
class UMWVMCombatUI;
class UUserWidget;
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
	virtual void NativePreConstruct() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

public:
	UFUNCTION(BlueprintCallable, Category = "MW|UI")
	void ClearCharacterPanelChildren();

	UFUNCTION(BlueprintCallable, Category = "MW|UI")
	UUserWidget* AddCharacterPanelChild();

	UFUNCTION(BlueprintCallable, Category = "MW|UI")
	UUserWidget* AddCharacterPanelChildByClass(TSubclassOf<UUserWidget> InChildClass);

private:
	void ResolveCombatUIViewModel();

	void BindSkillBarViewModels();

	void RebuildCharacterPanels();

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UVerticalBox> VB_CharacterPanel;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UMWCombatSkillBar> WBP_SkillBarLeft;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UMWCombatSkillBar> WBP_SkillBarRight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MW|UI")
	TSubclassOf<UUserWidget> CharacterPanelChildClass;

	UPROPERTY()
	TObjectPtr<UMWVMCombatUI> VMCombatUI = nullptr;
};
