#pragma once

// Include Header
#include "UI/MWUserWidget.h"
#include "MWRootCanvas.generated.h"

// Forward Declare
class UCanvasPanel;

// Define

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
	/* Add a new user widget to canvas. */
	bool AddWidget(UUserWidget* Widget, int32 ZOrder = 0);

	/* Remove a user widget from the canvas.*/
	bool RemoveWidget(UUserWidget* Widget);

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UCanvasPanel> CP_RootCanvas;
};