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
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FCommonImporterStyle::Initialize();
	FCommonImporterStyle::ReloadTextures();

	FCommonImporterCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	//PluginCommands->MapAction(
	//	FCommonImporterCommands::Get().CommonImporter,
	//	FExecuteAction::CreateRaw(this, &FCommonImporterModule::PluginButtonClicked),
	//	FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FCommonImporterModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(CommonImporterTabName, FOnSpawnTab::CreateRaw(this, &FCommonImporterModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("CommonImporter.FCommonImporterTabTitle", "CommonImporter"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);

	// Third Party
	InitThirdParty();
}

void FCommonImporterModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

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

	// ComboBox选项
	ComboBoxOptions.AddUnique(MakeShared<FString>("Excel"));
	ComboBoxOptions.AddUnique(MakeShared<FString>("JSON"));

	// 绑定选择变化
	SelectedOption = ComboBoxOptions[0]; // 默认选项

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
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
										.Text(FText::FromString(*Item)); // 显示下拉项内容
								})
								.OnSelectionChanged_Lambda([this](TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo)
								{
									SelectedOption = NewSelection; // 更新选中的选项
								})
								[
									SNew(STextBlock)
										.Text_Lambda([this]() 
										{ 
											return FText::FromString(*SelectedOption); 
										}) // 显示当前选中的项
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
								.Visibility_Lambda([this]() { return SelectedOption.IsValid() ? EVisibility::Visible : EVisibility::Hidden; }) // 控制按钮的可见性
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
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Tools");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("Tools");
			Section.AddMenuEntryWithCommandList(FCommonImporterCommands::Get().CommonImporter, PluginCommands);
		}
	}

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
	// get the  path of the asset by R.Clicking on the asset and 'Copy Reference'
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