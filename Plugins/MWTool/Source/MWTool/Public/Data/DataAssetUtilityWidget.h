#pragma once

// Include Header
#include "Blueprint/UserWidget.h"
#include "DataAssetUtilityWidget.generated.h"

// Forward Declare
class UButton;

// Define

/*
 * @class UDataAssetUtilityWidget
 * 
 * @brief
 *
 * @note
 */
UCLASS()
class UDataAssetUtilityWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	// Reload DataAsset, DataTable
	// データアセット、データテーブルをリロードする。
	UFUNCTION()
    void ReloadDataAsset();

protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> BTN_ReloadAsset;
};