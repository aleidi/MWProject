#include "UI/View/Common/MWMenuButton.h"
#include "UI/View/Common/MWButton.h"
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
		if (auto* slot = Cast<UHorizontalBoxSlot>(WBP_Button->Slot))
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
		if (auto* slot = Cast<UHorizontalBoxSlot>(WBP_Button->Slot))
		{
			slot->SetVerticalAlignment(TextVerticalAlignment);
		}
	}
}

UMWButton* UMWMenuButton::GetButton() const
{
	return WBP_Button.Get();
}

void UMWMenuButton::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(WBP_Button))
	{
		WBP_Button->OnHovered().AddUObject(this, &UMWMenuButton::OnHovered);
		WBP_Button->OnUnhovered().AddUObject(this, &UMWMenuButton::OnUnhovered);
		WBP_Button->OnFocusReceived().AddUObject(this, &UMWMenuButton::OnFocusReceived);
		WBP_Button->OnFocusLost().AddUObject(this, &UMWMenuButton::OnFocusLost);
		WBP_Button->OnInputMethodChangedDele.AddUObject(this, &UMWMenuButton::OnInputMethodChanged);
	}

	SetFocusSetting();
}

void UMWMenuButton::NativePreConstruct()
{
	Super::NativePreConstruct();

	SetIndentSize(IndentNormalSize);

	SetFocusSetting();

	if (IsValid(WBP_Button))
	{
		WBP_Button->SetButtonText(ButtonText);
	}

	if (IsValid(SB_ButtonSize))
	{
		SB_ButtonSize->SetWidthOverride(WidgetWidth);
		SB_ButtonSize->SetHeightOverride(WidgetHeight);
	}

	if (IsValid(WBP_Button))
	{
		SetTextHorizontalAlignment(TextHorizontalAlignment);
		SetTextVerticalAlignment(TextVerticalAlignment);
	}

	if(bUseImageAsBackGround && IsValid(BGImg))
	{
		MidBg = UMaterialInstanceDynamic::Create(BGImg, BD_BackGround, TEXT("BGImg"));
		BD_BackGround->SetBrushFromMaterial(MidBg);
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
	return IsAlignValid() && bUseIndent && IsValid(SP_Indent);
}

bool UMWMenuButton::IsAlignValid() const
{
	return IsValid(HB_Layout);
}

void UMWMenuButton::SetIndentSize(FSlateChildSize NewSize)
{
	if (IsIndentValid())
	{
		if (auto* slot = Cast<UHorizontalBoxSlot>(SP_Indent->Slot))
		{
			slot->SetSize(NewSize);
		}
	}
}

void UMWMenuButton::SetFocusSetting()
{
	if (IsValid(WBP_Button))
	{
		SetIsFocusable(true);

		SetDesiredFocusWidget(WBP_Button);
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
