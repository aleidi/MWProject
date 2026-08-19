#pragma once

// ヘッダーをインクルード
#include "UI/MWUserWidget.h"
#include "MWStartMenu.generated.h"

// 前方宣言
class UMWMenuButton;

// 定義

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