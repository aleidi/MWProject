#include "UI/ViewModel/MWMVVMContextNames.h"
#include "UI/ViewModel/Battle/MWVMCombatUI.h"

namespace MWMVVMContextNames
{
	const FName CombatUI(TEXT("MVVM.CombatUI"));

	const TArray<FGlobalViewModelDefinition>& GetGlobalDefinitions()
	{
		static const TArray<FGlobalViewModelDefinition> definitions =
		{
			{ CombatUI, UMWVMCombatUI::StaticClass()},
		};

		return definitions;
	}
}
