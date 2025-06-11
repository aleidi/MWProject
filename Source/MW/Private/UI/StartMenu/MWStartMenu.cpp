#include "UI/StartMenu/MWStartMenu.h"
#include "UI/Common/MWMenuButton.h"
#include "UI/Common/MWButton.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

void UMWStartMenu::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (IsValid(Start))
	{
		Start->GetButton()->OnPressed().AddUObject(this, &UMWStartMenu::OnStart);
	}

	if (IsValid(Exit))
	{
		Exit->GetButton()->OnPressed().AddUObject(this, &UMWStartMenu::OnExit);
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

	if (IsValid(Start))
	{
		SetDesiredFocusWidget(Start);
	}
	else if (IsValid(Continue))
	{
		SetDesiredFocusWidget(Continue);
	}
	else if (IsValid(Config))
	{
		SetDesiredFocusWidget(Config);
	}
	else if (IsValid(Exit))
	{
		SetDesiredFocusWidget(Exit);
	}
}