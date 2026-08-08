#pragma once

// Include Header
#include "UI/MWUserWidget.h"
#include "MWCombatSkillBar.generated.h"

// Forward Declare
class UMWCombatSkillInfo;
class UMWVMCharacterSkill;
class UMWVMSkillBar;

// Define

/*
 * @class UMWCombatSkillBar
 * 
 * @brief
 *
 * @note
 */
UCLASS()
class UMWCombatSkillBar : public UMWUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "MW|CombatSkillBar")
	void SetSkillBarViewModel(UMWVMSkillBar* InSkillBarViewModel);

protected:
	virtual void NativePreConstruct() override;

	virtual void NativeConstruct() override;

private:
	void RefreshSkillViewModelBindings();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UMWCombatSkillInfo> WBP_SkillUp;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UMWCombatSkillInfo> WBP_SkillLeft;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UMWCombatSkillInfo> WBP_SkillDown;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UMWCombatSkillInfo> WBP_SkillRight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MW|CombatSkillBar")
	float Radius = 100.0f;

	UPROPERTY()
	TObjectPtr<UMWVMSkillBar> VMSkillBar = nullptr;
};
