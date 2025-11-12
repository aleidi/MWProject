#include "Data/DataAssetUtilityWidget.h"
#include "Components/Button.h"
#include "Data/DataAsset/MWDataAsset.h"
#include "Data/MWDataTableManager.h"

void UDataAssetUtilityWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(BTN_ReloadAsset))
	{
		BTN_ReloadAsset->OnClicked.AddDynamic(this, &ThisClass::ReloadDataAsset);
	}
}

void UDataAssetUtilityWidget::ReloadDataAsset()
{
	DATAASSET->Reload();
	DATATABLEMANAGER->Reload();
}
