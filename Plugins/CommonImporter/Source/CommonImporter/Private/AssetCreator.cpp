#include "AssetCreator.h"
#include "AssetToolsModule.h"

UObject* UAssetCreator::CreateAsset(FString AssetPath, UClass* AssetClass, UFactory* AssetFactory, bool& bOutSuccess, FString& OutInfoMessage)
{
	// Get the asset tools module
	IAssetTools& asset_tools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();

	// Find right factory
	UFactory* factory = AssetFactory;
	if (factory == nullptr)
	{
		for (UFactory* fac : asset_tools.GetNewAssetFactories())
		{
			if (fac->SupportedClass == AssetClass)
			{
				factory = fac;
				break;
			}
		}

		if (factory == nullptr)
		{
			bOutSuccess = false;
			OutInfoMessage = FString::Printf(TEXT("Create asset failed."));
			return nullptr;
		}
	}

	if (factory->SupportedClass != AssetClass)
	{
		bOutSuccess = false;
		OutInfoMessage = FString::Printf(TEXT("Create asset failed."));
		return nullptr;
	}

	// Create asset
	UObject* asset = asset_tools.CreateAsset(FPaths::GetBaseFilename(AssetPath), FPaths::GetPath(AssetPath), AssetClass, factory);

	if (asset == nullptr)
	{
		bOutSuccess = false;
		OutInfoMessage = FString::Printf(TEXT("Create asset failed."));
		return nullptr;
	}

	bOutSuccess = true;
	OutInfoMessage = FString::Printf(TEXT("Create asset succeed."));
	return asset;
}