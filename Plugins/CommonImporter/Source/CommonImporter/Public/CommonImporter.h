// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Common.h"
#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;

class FCommonImporterModule : public IModuleInterface
{
public:

	/** IModuleInterface の実装 */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** コマンドにバインドされる処理（既定ではプラグインウィンドウを開く） */
	void PluginButtonClicked();
	
private:

	void InitThirdParty();
	void DeinitThirdParty();

	void RegisterMenus();

	void LoadExcelImporter();

	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);

private:
	TSharedPtr<class FUICommandList> PluginCommands;

	TArray<TSharedPtr<FString>> ComboBoxOptions; // ComboBox の選択肢
	TSharedPtr<FString> SelectedOption; // 現在選択中の項目

	void* Handlelibxl;
};
