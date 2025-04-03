#pragma once

#include "Common.h"
#include "BlueprintEditorLibrary.h"
#include "AssetCreator.generated.h"

UCLASS()
class COMMONIMPORTER_API UAssetCreator : public UBlueprintEditorLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "CommonImporter|Asset")
	static UObject* CreateAsset(FString AssetPath, UClass* AssetClass, UFactory* AssetFactory, bool& bOutSuccess);
};