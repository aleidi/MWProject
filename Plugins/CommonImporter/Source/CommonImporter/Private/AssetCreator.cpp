#include "AssetCreator.h"
#include "AssetToolsModule.h"

UObject* UAssetCreator::CreateAsset(FString AssetPath, UClass* AssetClass, UFactory* AssetFactory, bool& bOutSuccess)
{
	// AssetTools モジュールを取得
	IAssetTools& asset_tools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();

	// 適切なファクトリを探索
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
			UE_LOG(LogCommonImporter, Warning, TEXT("Asset factory is not existed."));
			return nullptr;
		}
	}

	if (factory->SupportedClass != AssetClass)
	{
		bOutSuccess = false;
		UE_LOG(LogCommonImporter, Warning, TEXT("Not supported class : %s"), *AssetClass->GetName());
		return nullptr;
	}

	// アセットを生成
	UObject* asset = asset_tools.CreateAsset(FPaths::GetBaseFilename(AssetPath), FPaths::GetPath(AssetPath), AssetClass, factory);

	if (asset == nullptr)
	{
		bOutSuccess = false;
		UE_LOG(LogCommonImporter, Warning, TEXT("asset instance creation failed : %s"), *AssetClass->GetName());
		return nullptr;
	}

	bOutSuccess = true;

	UE_LOG(LogCommonImporter, Warning, TEXT("asset instance creation suceeed : %s"), *asset->GetName());

	return asset;
}