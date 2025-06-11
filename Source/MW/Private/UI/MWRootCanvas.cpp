#include "UI/MWRootCanvas.h"
#include "Components/CanvasPanel.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanelSlot.h"
#include "MWLogChannels.h"

void UMWRootCanvas::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UMWRootCanvas::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UMWRootCanvas::NativeConstruct()
{
	Super::NativeConstruct();
}

bool UMWRootCanvas::AddWidget(UUserWidget* Widget, int32 ZOrder)
{
	if (!Widget)
	{
		UE_LOG(LogMWUI, Warning, TEXT("Add a null widget to root canvas panel."));

		return false;
	}

	UCanvasPanelSlot* slot = RootCanvas->AddChildToCanvas(Widget);

	slot->SetAnchors(FAnchors(0.f, 0.f, 1.f, 1.f));

	slot->SetZOrder(ZOrder);

	return true;
}

bool UMWRootCanvas::RemoveWidget(UUserWidget* Widget)
{
	return RootCanvas->RemoveChild(Widget);
}
