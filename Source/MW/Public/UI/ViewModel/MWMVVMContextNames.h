#pragma once

// Forward Declare
class UMVVMViewModelBase;

namespace MWMVVMContextNames
{
	struct FGlobalViewModelDefinition
	{
		FName ContextName = NAME_None;
		TSubclassOf<UMVVMViewModelBase> ViewModelClass = nullptr;
	};

	// ==== Global ViewModel Context ====
	extern const FName CombatUI;

	const TArray<FGlobalViewModelDefinition>& GetGlobalDefinitions();
}
