#include "UI/View/Common/MWButton.h"
#include "CommonTextBlock.h"
#include "Components/Spacer.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Blueprint/WidgetTree.h"
#include "MWLogChannels.h"

void UMWButton::SetButtonText(FText NewText)
{
	ButtonText = NewText;

	if (IsValid(CTB_BtnText))
	{
		CTB_BtnText->SetText(ButtonText);
	}
}

FText UMWButton::GetButtonText() const
{
	return ButtonText;
}

void UMWButton::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMWButton::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (IsValid(CTB_BtnText))
	{
		CTB_BtnText->SetText(ButtonText);
	}
}

void UMWButton::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (IsValid(CTB_BtnText))
	{
		CTB_BtnText->SetStyle(GetCurrentTextStyleClass());
	}
}

void UMWButton::NativeOnCurrentTextStyleChanged()
{
	Super::NativeOnCurrentTextStyleChanged();

	if (IsValid(CTB_BtnText))
	{
		CTB_BtnText->SetStyle(GetCurrentTextStyleClass());
	}
}

void UMWButton::OnInputMethodChanged(ECommonInputType CurrentInputType)
{
	Super::OnInputMethodChanged(CurrentInputType);
}
