#pragma once

// Include Header
#include "UI/MWUserWidget.h"
#include "MWStartMenu.generated.h"

// Forward Declare
class UMWMenuButton;

// Define

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
	TObjectPtr<UMWMenuButton> WBP_Start;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UMWMenuButton> WBP_Continue;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UMWMenuButton> WBP_Config;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UMWMenuButton> WBP_Exit;

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