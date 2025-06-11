#pragma once

// Include Header
#include "UI/MWUserWidget.h"
#include "MWStartMenu.generated.h"

// Forward Declare
class UMWMenuButton;

// Macro

/*
 * @class UMWStartMenu
 * 
 * @brief
 *
 * @note
 */
UCLASS()
class UMWStartMenu : public UMWUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UMWMenuButton> Start;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UMWMenuButton> Continue;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UMWMenuButton> Config;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UMWMenuButton> Exit;

	UPROPERTY(EditDefaultsOnly, Category = "Menu")
	TSoftObjectPtr<UWorld> Level;

protected:
	virtual void NativePreConstruct() override;

	virtual void NativeConstruct() override;

	void OnStart();

	void OnExit();

private:
	void SetFocus();

};