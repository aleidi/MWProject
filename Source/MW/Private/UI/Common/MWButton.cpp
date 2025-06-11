#include "UI/Common/MWButton.h"
#include "CommonTextBlock.h"
#include "Components/Spacer.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Blueprint/WidgetTree.h"
#include "MWLogChannels.h"

void UMWButton::SetButtonText(FText NewText)
{
	ButtonText = NewText;

	if (IsValid(Txt_BtnText))
	{
		Txt_BtnText->SetText(ButtonText);
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

	if (IsValid(Txt_BtnText))
	{
		Txt_BtnText->SetText(ButtonText);
	}
}

void UMWButton::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (IsValid(Txt_BtnText))
	{
		Txt_BtnText->SetStyle(GetCurrentTextStyleClass());
	}
}

void UMWButton::NativeOnCurrentTextStyleChanged()
{
	Super::NativeOnCurrentTextStyleChanged();

	if (IsValid(Txt_BtnText))
	{
		Txt_BtnText->SetStyle(GetCurrentTextStyleClass());
	}
}

void UMWButton::OnInputMethodChanged(ECommonInputType CurrentInputType)
{
	Super::OnInputMethodChanged(CurrentInputType);
}