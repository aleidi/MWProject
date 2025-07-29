#pragma once

// Include Header
#include "Engine/DataAsset.h"
#include "Define/MWDefineUI.h"
#include "MWUIConfigData.generated.h"


UCLASS()
class UMWUILayerConfig : public UDataAsset 
{
	GENERATED_BODY()

public:	
	UPROPERTY(EditDefaultsOnly)
	TMap<EMWUILayer, int32> Config;
};

UCLASS()
class UMWUIConfigData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> RootCanvas;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UMWUILayerConfig> LayerConfigData;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UDataTable> ClassTable;
};