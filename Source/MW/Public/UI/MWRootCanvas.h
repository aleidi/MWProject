#pragma once

// ヘッダーをインクルード
#include "UI/MWUserWidget.h"
#include "MWRootCanvas.generated.h"

// 前方宣言
class UCanvasPanel;

// 定義

/*
 * @class UMWRootCanvas
 * 
 * @brief
 *
 * @note
 */
UCLASS()
class UMWRootCanvas : public UMWUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	virtual void NativePreConstruct() override;

	virtual void NativeConstruct() override;

public:
	/* 新しいUserWidgetをCanvasへ追加します。 */
	bool AddWidget(UUserWidget* Widget, int32 ZOrder = 0);

	/* UserWidgetをCanvasから削除します。 */
	bool RemoveWidget(UUserWidget* Widget);

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UCanvasPanel> CP_RootCanvas;
};