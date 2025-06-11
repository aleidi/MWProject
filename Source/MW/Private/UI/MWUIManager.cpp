#include "UI/MWUIManager.h"
#include "System/MWAssetManager.h"
#include "Data/MWMasterData.h"
#include "MWLogChannels.h"
#include "UI/MWRootCanvas.h"
#include "Gamemode/MWGameModeBase.h"

void UMWUIManager::Initialize(FSubsystemCollectionBase& Collection)
{
	UE_LOG(LogMWUI, Log, TEXT("[%s] world : %s created."), UTF8_TO_TCHAR(__FUNCTION__), GetWorld() ? *GetWorld()->GetName() : TEXT("null"));

	LoadDataConfig();

	AMWGameModeBase::OnGameBeginPlay.AddUObject(this, &UMWUIManager::OnGameModeBeginPlay);
}

void UMWUIManager::Deinitialize()
{
	UE_LOG(LogMWUI, Log, TEXT("[%s] current world is : %s."), UTF8_TO_TCHAR(__FUNCTION__), GetWorld() ? *GetWorld()->GetName() : TEXT("null"));
}

bool UMWUIManager::OpenUI(const FName& WidgetName, bool bFocus)
{
	if (!HasRootCanvas())
	{
		// try create canvas if it's not created
		if (!CreateCanvas())
		{
			return false;
		}
	}

	FMWWidgetLayerInfo* widgetInfo = WidgetInfoMap.Find(WidgetName);
	if (!widgetInfo)
	{
		UE_LOG(LogMWUI, Warning, TEXT("%s widget is not registered. Please check MWUIClassConfig or the widget's name."), *WidgetName.ToString());

		return false;
	}

	if (widgetInfo->Widget.IsNull())
	{
		UE_LOG(LogMWUI, Warning, TEXT("the widget class of %s is not set. Please check MWUIClassConfig."), *WidgetName.ToString());

		return false;
	}

	UUserWidget* widgetInst = nullptr;

	auto* foundWidget = CachedWidgetInst.Find(WidgetName);

	const bool bWidgetCached = foundWidget != nullptr;

	if (!bWidgetCached)
	{
		widgetInfo->Widget.LoadSynchronous();

		// try to create a new widget
		widgetInst = CreateWidget(GetWorld(), widgetInfo->Widget.Get());

		check(widgetInst);
	}
	else
	{
		if (foundWidget->Get()->GetParent())
		{
			UE_LOG(LogMWUI, Warning, TEXT("%s widget is already add to screen"), *WidgetName.ToString());

			return false;
		}

		widgetInst = *foundWidget;
	}

	// Get Z Order
	int32* layerCfg = LayerConfig.Find(widgetInfo->Layer);
	if (!layerCfg)
	{
		UE_LOG(LogMWUI, Warning, TEXT("%s is not defined in layer config. Please check MWUILayerConfig."), *UEnum::GetDisplayValueAsText(widgetInfo->Layer).ToString());
	}

	const int32 zorder = layerCfg ? *layerCfg + widgetInfo->LayerOffset : -1;

	if (!RootCanvas->AddWidget(widgetInst, zorder))
	{	
		return false;
	}

	if (!bWidgetCached)
	{
		CachedWidgetInst.Emplace(WidgetName, widgetInst);
	}

	if (auto* uiInterface = Cast<IMWUIInterface>(widgetInst))
	{
		uiInterface->PlayOpenAnim();
	}

	if (bFocus && widgetInst->IsFocusable() && widgetInst->GetDesiredFocusWidget())
	{
		widgetInst->SetFocus();
	}
	
	return true;
}

bool UMWUIManager::CloseUI(const FName& WidgetName, bool bForceImmediately)
{
	if (!HasRootCanvas())
	{
		return false;
	}

	auto* foundWidget = CachedWidgetInst.Find(WidgetName);

	if (!foundWidget || !foundWidget->Get()->GetParent())
	{
		return false;
	}
	
	if (auto* uiInterface = Cast<IMWUIInterface>(*foundWidget))
	{
		if (!bForceImmediately)
		{
			uiInterface->PlayCloseAnim([this, foundWidget]()
			{
				RootCanvas->RemoveWidget(*foundWidget);
			});

			return true;
		}
	}

	return RootCanvas->RemoveWidget(*foundWidget);
}

void UMWUIManager::OnGameModeBeginPlay(AGameModeBase* NewGameMode)
{
	CreateCanvas();
}

bool UMWUIManager::HasRootCanvas()
{
	return IsValid(RootCanvas);
}

bool UMWUIManager::CreateCanvas()
{
	const UMWMasterData& data = UMWAssetManager::Get().GetMasterData();
	
	if (data.UIConfig && data.UIConfig->RootCanvas)
	{
		RootCanvas = Cast<UMWRootCanvas>(CreateWidget(GetWorld(), data.UIConfig->RootCanvas));

		if (!RootCanvas)
		{
			UE_LOG(LogMWUI, Warning, TEXT("Root canvas creation failed!"));

			return false;
		}

		RootCanvas->AddToViewport();

		return true;
	}

	return false;
}

void UMWUIManager::LoadDataConfig()
{
	const UMWMasterData& data = UMWAssetManager::Get().GetMasterData();

	if (data.UIConfig && data.UIConfig->ClassTable)
	{
		TArray<FMWUIClassConfig*> rows;

		data.UIConfig->ClassTable->GetAllRows("", rows);

		for (auto row : rows)
		{
			WidgetInfoMap.Emplace(row->Name, row->WidgetConfig);
		}
	}

	if (data.UIConfig && data.UIConfig->LayerConfigData)
	{
		LayerConfig = data.UIConfig->LayerConfigData->Config;
	}
}
