#include "UI/Common/MWMenuButton.h"
#include "UI/Common/MWButton.h"
#include "CommonTextBlock.h"
#include "Components/Spacer.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Blueprint/WidgetTree.h"
#include "Components/SizeBox.h"
#include "Animation/WidgetAnimation.h"
#include "Components/Border.h"
#include "MWLogChannels.h"
#include "Materials/MaterialInstanceDynamic.h"

void UMWMenuButton::SetTextHorizontalAlignment(EHorizontalAlignment NewHorizontalAlignment)
{
	TextHorizontalAlignment = NewHorizontalAlignment;

	if (IsAlignValid())
	{
		if (auto* slot = Cast<UHorizontalBoxSlot>(Button->Slot))
		{		
			slot->SetHorizontalAlignment(TextHorizontalAlignment);
		}
	}
}

void UMWMenuButton::SetTextVerticalAlignment(EVerticalAlignment NewVerticalAlignment)
{
	TextVerticalAlignment = NewVerticalAlignment;

	if (IsAlignValid())
	{
		if (auto* slot = Cast<UHorizontalBoxSlot>(Button->Slot))
		{
			slot->SetVerticalAlignment(TextVerticalAlignment);
		}
	}
}

UMWButton* UMWMenuButton::GetButton() const
{
	return Button.Get();
}

void UMWMenuButton::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(Button))
	{
		Button->OnHovered().AddUObject(this, &UMWMenuButton::OnHovered);
		Button->OnUnhovered().AddUObject(this, &UMWMenuButton::OnUnhovered);
		Button->OnFocusReceived().AddUObject(this, &UMWMenuButton::OnFocusReceived);
		Button->OnFocusLost().AddUObject(this, &UMWMenuButton::OnFocusLost);
		Button->OnInputMethodChangedDele.AddUObject(this, &UMWMenuButton::OnInputMethodChanged);
	}

	SetFocusSetting();
}

void UMWMenuButton::NativePreConstruct()
{
	Super::NativePreConstruct();

	SetIndentSize(IndentNormalSize);

	SetFocusSetting();

	if (IsValid(Button))
	{
		Button->SetButtonText(ButtonText);
	}

	if (IsValid(SizeBox))
	{
		SizeBox->SetWidthOverride(WidgetWidth);
		SizeBox->SetHeightOverride(WidgetHeight);
	}

	SetTextHorizontalAlignment(TextHorizontalAlignment);
	SetTextVerticalAlignment(TextVerticalAlignment);

	if(bUseImageAsBackGround && IsValid(BGImg))
	{
		MidBg = UMaterialInstanceDynamic::Create(BGImg, BG, TEXT("BGImg"));
		BG->SetBrushFromMaterial(MidBg);
	}
}

void UMWMenuButton::OnHovered()
{
	IndentAndEnableFlicker();
}

void UMWMenuButton::OnUnhovered()
{
	UnindentAndDisableFlicker();
}

void UMWMenuButton::OnFocusReceived()
{
	IndentAndEnableFlicker();
}

void UMWMenuButton::OnFocusLost()
{
	UnindentAndDisableFlicker();
}

bool UMWMenuButton::IsIndentValid() const
{
	return IsAlignValid() && bUseIndent && IsValid(Indent);
}

bool UMWMenuButton::IsAlignValid() const
{
	return IsValid(HorizontalBox);
}

void UMWMenuButton::SetIndentSize(FSlateChildSize NewSize)
{
	if (IsIndentValid())
	{
		if (auto* slot = Cast<UHorizontalBoxSlot>(Indent->Slot))
		{
			slot->SetSize(NewSize);
		}
	}
}

void UMWMenuButton::SetFocusSetting()
{
	if (IsValid(Button))
	{
		SetIsFocusable(true);

		SetDesiredFocusWidget(Button);
	}
}

void UMWMenuButton::EnableBGFlicker()
{
	if (MidBg)
	{
		MidBg->SetScalarParameterValue(FlickerVarName, FlickerFrequenc);
	}
}

void UMWMenuButton::DisableBGFlicker()
{
	if (MidBg)
	{
		MidBg->SetScalarParameterValue(FlickerVarName, 0.f);
	}
}

void UMWMenuButton::IndentAndEnableFlicker()
{
	SetIndentSize(IndentHoverSize);

	DisableBGFlicker();

	FTimerManager& timerMng = GetWorld()->GetTimerManager();

	timerMng.SetTimerForNextTick(this, &UMWMenuButton::EnableBGFlicker);
}

void UMWMenuButton::UnindentAndDisableFlicker()
{
	SetIndentSize(IndentNormalSize);

	DisableBGFlicker();
}

void UMWMenuButton::OnInputMethodChanged(ECommonInputType CurrentInputType)
{
	UnindentAndDisableFlicker();
}
