#include "UI/StartMenu/MWStartMenu.h"
#include "UI/Common/MWMenuButton.h"
#include "UI/Common/MWButton.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

void UMWStartMenu::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (IsValid(WBP_Start))
	{
		WBP_Start->GetButton()->OnPressed().AddUObject(this, &UMWStartMenu::OnStart);
	}

	if (IsValid(WBP_Exit))
	{
		WBP_Exit->GetButton()->OnPressed().AddUObject(this, &UMWStartMenu::OnExit);
	}
}

void UMWStartMenu::NativeConstruct()
{
	Super::NativeConstruct();

	SetFocus();
}

void UMWStartMenu::OnStart()
{
	if (!Level.IsNull())
	{
		UGameplayStatics::OpenLevelBySoftObjectPtr(this, Level);
	}
}

void UMWStartMenu::OnExit()
{
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, false);
}

void UMWStartMenu::SetFocus()
{
	SetIsFocusable(true);

	if (IsValid(WBP_Start))
	{
		SetDesiredFocusWidget(WBP_Start);
	}
	else if (IsValid(WBP_Continue))
	{
		SetDesiredFocusWidget(WBP_Continue);
	}
	else if (IsValid(WBP_Config))
	{
		SetDesiredFocusWidget(WBP_Config);
	}
	else if (IsValid(WBP_Exit))
	{
		SetDesiredFocusWidget(WBP_Exit);
	}
}