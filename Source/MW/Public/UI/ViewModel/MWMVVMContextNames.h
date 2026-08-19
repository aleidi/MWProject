#pragma once

// 前方宣言
class UMVVMViewModelBase;

namespace MWMVVMContextNames
{
	struct FGlobalViewModelDefinition
	{
		FName ContextName = NAME_None;
		TSubclassOf<UMVVMViewModelBase> ViewModelClass = nullptr;
	};

	// ==== グローバルViewModelコンテキスト ====
	extern const FName CombatUI;

	const TArray<FGlobalViewModelDefinition>& GetGlobalDefinitions();
}
