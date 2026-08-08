#include "UI/View/Battle/MWCombatSkillBar.h"

#include "Components/CanvasPanelSlot.h"
#include "UI/View/Battle/MWCombatSkillInfo.h"
#include "UI/ViewModel/Battle/MWVMCharacterSkill.h"
#include "UI/ViewModel/Battle/MWVMSkillBar.h"

// Define
namespace
{
	const int32 SkillSlotUp = 0;
	const int32 SkillSlotLeft = 1;
	const int32 SkillSlotDown = 2;
	const int32 SkillSlotRight = 3;

	void LayoutDiamond(UWidget* Up, UWidget* Left, UWidget* Down, UWidget* Right, float Radius)
	{
		auto setOffset = [&](UWidget* Widget, const FVector2D& Offset)
		{
			if (!Widget)
			{

				return;
			}

			Widget->SetRenderTransformPivot(FVector2D(0.5f, 0.5f));

			FWidgetTransform transform = Widget->GetRenderTransform();
			transform.Translation = Offset;
			Widget->SetRenderTransform(transform);
		};

		setOffset(Up, FVector2D(0.0f, -Radius));
		setOffset(Left,FVector2D(-Radius, 0.0f));
		setOffset(Down, FVector2D(0.0f, Radius));
		setOffset(Right, FVector2D(Radius, 0.0f));
	}
}

void UMWCombatSkillBar::SetSkillBarViewModel(UMWVMSkillBar* InSkillBarViewModel)
{
	VMSkillBar = InSkillBarViewModel;

	RefreshSkillViewModelBindings();
}

void UMWCombatSkillBar::NativePreConstruct()
{
	Super::NativePreConstruct();

	LayoutDiamond(WBP_SkillUp, WBP_SkillLeft, WBP_SkillDown, WBP_SkillRight, Radius);
}

void UMWCombatSkillBar::NativeConstruct()
{
	Super::NativeConstruct();

	RefreshSkillViewModelBindings();
}

void UMWCombatSkillBar::RefreshSkillViewModelBindings()
{
	if (!VMSkillBar)
	{
		return;
	}

	auto bindSkill = [&](UMWCombatSkillInfo* skillWidget, int32 slotIndex)
		{
			if (!skillWidget)
			{
				return;
			}

			UMWVMCharacterSkill* skillVM = VMSkillBar->GetSkillByIndex(slotIndex);
			if (!skillVM)
			{
				return;
			}

			const FName viewModelName = skillWidget->GetManualViewModelNameByClass(UMWVMCharacterSkill::StaticClass());
			if (viewModelName.IsNone())
			{
				return;
			}

			skillWidget->SetViewModel(viewModelName, skillVM, true);
		};

	bindSkill(WBP_SkillUp, SkillSlotUp);
	bindSkill(WBP_SkillLeft, SkillSlotLeft);
	bindSkill(WBP_SkillDown, SkillSlotDown);
	bindSkill(WBP_SkillRight, SkillSlotRight);
}
