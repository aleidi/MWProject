#include "UI/Battle/MWCombatUI.h"
#include "Components/VerticalBox.h"
#include "Components/Image.h"

void UMWCombatUI::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (IsValid(VB_CharacterPanel))
	{
		UImage* img = NewObject<UImage>();
		VB_CharacterPanel->AddChild(img);
		VB_CharacterPanel->AddChild(NewObject<UImage>());
	}
}

void UMWCombatUI::NativeConstruct()
{
	Super::NativeConstruct();
}
