#include "UI/MWUIManager.h"
#include "Data/MWGameplayData.h"
#include "Data/MWUIConfigData.h"
#include "Gamemode/MWGameModeBase.h"
#include "MWGameSingleton.h"
#include "MWLogChannels.h"
#include "System/MWGameInstanceSubsystem.h"
#include "UI/MWRootCanvas.h"

void UMWUIManager::Initialize()
{
	OnGameModeBeginPlayHandle = AMWGameModeBase::OnGameBeginPlay.AddUObject(this, &UMWUIManager::OnGameModeBeginPlay);
}

void UMWUIManager::Deinitialize()
{
	if (OnGameModeBeginPlayHandle.IsValid())
	{
		AMWGameModeBase::OnGameBeginPlay.Remove(OnGameModeBeginPlayHandle);
	}
}

UMWUIManager* UMWUIManager::Get(const UObject* WorldContext)
{
	if (UMWGameInstanceSubsystem* subsystem = UMWGameInstanceSubsystem::Get(WorldContext))
	{
		return subsystem->GetUIManager();
	}

	return nullptr;
}

UMWUIManager::UMWUIManager()
{
}

bool UMWUIManager::OpenUI(const FName& WidgetName, bool bFocus)
{
	if (!HasRootCanvas())
	{
		// 未作成ならCanvasの作成を試行
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

		// 新規Widgetの作成を試行
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

	// Z Orderを取得
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

	LoadDataConfig();
}

bool UMWUIManager::HasRootCanvas()
{
	return IsValid(RootCanvas);
}

bool UMWUIManager::CreateCanvas()
{
	const UMWGameplayData* data = GET_MWSINGLETON()->GetGameplayData();
	
	if (data->UIConfig && data->UIConfig->RootCanvas)
	{
		RootCanvas = Cast<UMWRootCanvas>(CreateWidget(GetWorld(), data->UIConfig->RootCanvas));

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
	const UMWGameplayData* data = GET_MWSINGLETON()->GetGameplayData();

	if (data->UIConfig && data->UIConfig->ClassTable)
	{
		TArray<FMWUIClassConfig*> rows;

		data->UIConfig->ClassTable->GetAllRows("", rows);

		for (auto row : rows)
		{
			WidgetInfoMap.Emplace(row->Name, row->WidgetConfig);
		}
	}

	if (data->UIConfig && data->UIConfig->LayerConfigData)
	{
		LayerConfig = data->UIConfig->LayerConfigData->Config;
	}
}
