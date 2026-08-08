#include "UI/View/Battle/MWCombatUI.h"

#include "Blueprint/UserWidget.h"
#include "Components/VerticalBox.h"
#include "UI/View/Battle/MWCombatCharacterPanel.h"
#include "UI/View/Battle/MWCombatSkillBar.h"
#include "UI/ViewModel/Battle/MWVMCharacterGroup.h"
#include "UI/ViewModel/Battle/MWVMCombatUI.h"
#include "UI/ViewModel/Battle/MWVMSkillBar.h"
#include "UI/ViewModel/MWMVVMContextNames.h"
#include "UI/ViewModel/MWViewModelManager.h"

void UMWCombatUI::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UMWCombatUI::NativeConstruct()
{
	Super::NativeConstruct();

	ResolveCombatUIViewModel();

	if (VMCombatUI)
	{
		VMCombatUI->Initialize(this);
	}

	BindSkillBarViewModels();

	RebuildCharacterPanels();
}

void UMWCombatUI::NativeDestruct()
{
	if (VMCombatUI)
	{
		VMCombatUI->Deinitialize();
	}

	Super::NativeDestruct();
}

void UMWCombatUI::ClearCharacterPanelChildren()
{
	if (!VB_CharacterPanel)
	{
		return;
	}

	VB_CharacterPanel->ClearChildren();
}

UUserWidget* UMWCombatUI::AddCharacterPanelChild()
{
	return AddCharacterPanelChildByClass(CharacterPanelChildClass);
}

UUserWidget* UMWCombatUI::AddCharacterPanelChildByClass(TSubclassOf<UUserWidget> InChildClass)
{
	if (!VB_CharacterPanel || !InChildClass)
	{
		return nullptr;
	}

	if (UUserWidget* childWidget = CreateWidget<UUserWidget>(this, InChildClass))
	{
		VB_CharacterPanel->AddChildToVerticalBox(childWidget);

		return childWidget;
	}

	return nullptr;
}

void UMWCombatUI::ResolveCombatUIViewModel()
{
	VMCombatUI = GetViewModelByClass<UMWVMCombatUI>();

	if (VMCombatUI)
	{
		return;
	}

	if (UMWViewModelManager* vmManager = UMWViewModelManager::Get(this))
	{
		VMCombatUI = vmManager->FindGlobalViewModel<UMWVMCombatUI>(MWMVVMContextNames::CombatUI);

		//SetViewModel(MWMVVMContextNames::CombatUI, VMCombatUI);
	}
}

void UMWCombatUI::BindSkillBarViewModels()
{
	if (!VMCombatUI)
	{
		return;
	}

	if (WBP_SkillBarLeft)
	{
		WBP_SkillBarLeft->SetSkillBarViewModel(VMCombatUI->GetLeftSkillBar());
	}

	if (WBP_SkillBarRight)
	{
		WBP_SkillBarRight->SetSkillBarViewModel(VMCombatUI->GetRightSkillBar());
	}

	//if (WBP_SkillBarLeft)
	//{
	//	const FName vmName = WBP_SkillBarLeft->GetManualViewModelNameByClass(UMWVMSkillBar::StaticClass());
	//	if (!vmName.IsNone())
	//	{
	//		WBP_SkillBarLeft->SetViewModel(vmName, VMCombatUI->GetLeftSkillBar());
	//	}
	//}

	//if (WBP_SkillBarRight)
	//{
	//	const FName vmName = WBP_SkillBarRight->GetManualViewModelNameByClass(UMWVMSkillBar::StaticClass());
	//	if (!vmName.IsNone())
	//	{
	//		WBP_SkillBarRight->SetViewModel(vmName, VMCombatUI->GetRightSkillBar());
	//	}
	//}
}

void UMWCombatUI::RebuildCharacterPanels()
{
	if (!VMCombatUI)
	{
		return;
	}

	ClearCharacterPanelChildren();

	const TArray<TObjectPtr<UMWVMCharacterGroup>>& groupList = VMCombatUI->GetCharacterStatusList();

	for (const TObjectPtr<UMWVMCharacterGroup>& characterGroupVM : groupList)
	{
		if (!characterGroupVM)
		{
			continue;
		}

		UUserWidget* childWidget = AddCharacterPanelChild();

		UMWCombatCharacterPanel* characterPanel = Cast<UMWCombatCharacterPanel>(childWidget);

		if (!characterPanel)
		{
			continue;
		}

		characterPanel->SetCharacterGroupViewModel(characterGroupVM);
	}
}
