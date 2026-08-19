// Copyright Epic Games, Inc. All Rights Reserved.

#include "CommonImporter.h"
#include "CommonImporterStyle.h"
#include "CommonImporterCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"
#include "Engine/CurveTable.h"
#include "Interfaces/IPluginManager.h"

#include "EditorUtilitySubsystem.h"
#include "Editor/Blutility/Classes/EditorUtilityWidget.h"
#include "Editor/UMGEditor/Public/WidgetBlueprint.h"
#include "Editor/Blutility/Classes/EditorUtilityWidgetBlueprint.h"

static const FName CommonImporterTabName("CommonImporter");

#define LOCTEXT_NAMESPACE "FCommonImporterModule"

void FCommonImporterModule::StartupModule()
{
	// この処理はモジュール読み込み後に実行される（正確なタイミングは .uplugin のモジュール設定で定義）。
	
	FCommonImporterStyle::Initialize();
	FCommonImporterStyle::ReloadTextures();

	FCommonImporterCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FCommonImporterCommands::Get().CommonImporter,
		FExecuteAction::CreateRaw(this, &FCommonImporterModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FCommonImporterModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(CommonImporterTabName, FOnSpawnTab::CreateRaw(this, &FCommonImporterModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("CommonImporter.FCommonImporterTabTitle", "CommonImporter"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);

	// サードパーティライブラリを初期化
	InitThirdParty();
}

void FCommonImporterModule::ShutdownModule()
{
	// シャットダウン時のクリーンアップ処理。
	// 動的リロード対応モジュールでは、アンロード前に呼び出される。

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FCommonImporterStyle::Shutdown();

	FCommonImporterCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(CommonImporterTabName);
}

TSharedRef<SDockTab> FCommonImporterModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	return SNew(SDockTab);
	FText WidgetText = FText::Format(
		LOCTEXT("CommonImporter.WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FCommonImporterModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("CommonImporter.cpp"))
		);

	// ComboBox の選択肢
	ComboBoxOptions.AddUnique(MakeShared<FString>("Excel"));
	ComboBoxOptions.AddUnique(MakeShared<FString>("JSON"));

	// 選択変更をバインド
	SelectedOption = ComboBoxOptions[0]; // 既定の選択項目

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// タブのコンテンツをここに配置
			SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.Padding(5)
				[
					SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						.Padding(5)
						+ SHorizontalBox::Slot()
						.HAlign(HAlign_Left)
						.VAlign(VAlign_Center)
						[
							SNew(SComboBox<TSharedPtr<FString>>)
								.OptionsSource(&ComboBoxOptions)
								.OnGenerateWidget_Lambda([this](TSharedPtr<FString> Item)
								{
									return SNew(STextBlock)
										.Text(FText::FromString(*Item)); // ドロップダウン項目を表示
								})
								.OnSelectionChanged_Lambda([this](TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo)
								{
									SelectedOption = NewSelection; // 選択中の項目を更新
								})
								[
									SNew(STextBlock)
										.Text_Lambda([this]() 
										{ 
											return FText::FromString(*SelectedOption); 
										}) // 現在の選択項目を表示
								]
						]
						+ SHorizontalBox::Slot()
						.HAlign(HAlign_Right)
						.VAlign(VAlign_Center)
						[
							SNew(SButton)
								.Text(FText::FromString("Execute"))
								.OnClicked_Lambda([this]()
								{
									if (SelectedOption.IsValid())
									{
										FText OutputText = FText::Format(FText::FromString("You selected: {0}"), FText::FromString(*SelectedOption));
										FMessageDialog::Open(EAppMsgType::Ok, OutputText);
									}
									return FReply::Handled();
								})
								.Visibility_Lambda([this]() { return SelectedOption.IsValid() ? EVisibility::Visible : EVisibility::Hidden; }) // ボタンの表示可否を制御
						]
				]
		];
}

void FCommonImporterModule::PluginButtonClicked()
{
	LoadExcelImporter();

	//FGlobalTabmanager::Get()->TryInvokeTab(CommonImporterTabName);
}

void FCommonImporterModule::InitThirdParty()
{
	TSharedPtr<IPlugin> module = IPluginManager::Get().FindPlugin("CommonImporter");

	if (module.IsValid())
	{
		const FString dll_path = module->GetBaseDir() + TEXT("/Source/CommonImporter/ThirdParty/libxl/bin64/libxl.dll");
		Handlelibxl = FPlatformProcess::GetDllHandle(*dll_path);
		if (!Handlelibxl)
		{
			UE_LOG(LogCommonImporter, Warning, TEXT("CommonImporter plguin : failed to load libxl dll."));
		}
	}
	else
	{
		UE_LOG(LogCommonImporter, Warning, TEXT("CommonImporter plguin : not found the plugin."));
	}
}

void FCommonImporterModule::DeinitThirdParty()
{
	if (Handlelibxl)
	{
		FPlatformProcess::FreeDllHandle(Handlelibxl);
	}
}

void FCommonImporterModule::RegisterMenus()
{
	// Owner は UToolMenus::UnregisterOwner 実行時のクリーンアップ対象として使用される
	FToolMenuOwnerScoped OwnerScoped(this);

	//{
	//	UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Tools");
	//	{
	//		FToolMenuSection& Section = Menu->FindOrAddSection("Tools");
	//		Section.AddMenuEntryWithCommandList(FCommonImporterCommands::Get().CommonImporter, PluginCommands);
	//	}
	//}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("CommonImporter");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(
					FCommonImporterCommands::Get().CommonImporter,
					LOCTEXT("CommonImporter_Label", "CommonImporter"),
					LOCTEXT("CommonImporter_Tooltip", "Import excel to UE as data table"),
					FSlateIcon("CommonImporterStyle", "CommonImporter.Logo")
					));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

void FCommonImporterModule::LoadExcelImporter()
{
	// アセットを右クリックして「Copy Reference」を実行し、参照パスを取得する
	const FSoftObjectPath widgetAssetPath("/CommonImporter/LoadExcel.LoadExcel");

	UObject* widgetAssetLoaded = widgetAssetPath.TryLoad();
	if (widgetAssetLoaded == nullptr) {
		UE_LOG(LogCommonImporter, Warning, TEXT("Missing Expected widget class at : /CommonImporter/LoadExcel.LoadExcel"));
		return;
	}

	UEditorUtilityWidgetBlueprint* widget = Cast<UEditorUtilityWidgetBlueprint>(widgetAssetLoaded);
	if (widget == nullptr) {
		UE_LOG(LogCommonImporter, Warning, TEXT("Couldnt cast /CommonImporter/LoadExcel.LoadExcel to UEditorUtilityWidgetBlueprint"));
		return;
	}

	UEditorUtilitySubsystem* EditorUtilitySubsystem = GEditor->GetEditorSubsystem<UEditorUtilitySubsystem>();
	EditorUtilitySubsystem->SpawnAndRegisterTab(widget);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCommonImporterModule, CommonImporter)