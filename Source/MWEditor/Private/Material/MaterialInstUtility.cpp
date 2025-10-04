#include "Material/MaterialInstUtility.h"
#include "EditorUtilityLibrary.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Factories/MaterialInstanceConstantFactoryNew.h"
#include "PackageTools.h"
#include "AssetRegistry/AssetRegistryModule.h"

void UMaterialInstUtility::CreateMaterialInstaFromMaterial(UMaterialInstance* TemplateMaterialInst)
{
	if (!TemplateMaterialInst)
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("TemplateMaterialInst is nullptr!")));

		return;
	}

	TArray<UObject*> selectedAssets = UEditorUtilityLibrary::GetSelectedAssets();

	TArray<FMaterialInstanceCreateInfo> createInfos;

	for (UObject* obj : selectedAssets)
	{
		if (UMaterialInterface* material = Cast<UMaterialInterface>(obj))
		{
			FMaterialInstanceCreateInfo tmpreateInfo;

			// 1. Set new asset name and path
			FString assetName = material->GetName();

			// Change the prefix "M_" to "MI_"
			if (assetName.StartsWith(TEXT("M_")))
			{
				assetName = assetName.Replace(TEXT("M_"), TEXT("MI_"));
			}
			else
			{
				assetName = FString::Printf(TEXT("MI_%s"), *assetName);
			}
			
			FString assetPath = material->GetPackage()->GetName();

			// Remove the asset name from the path
			assetPath = FPaths::GetPath(assetPath);

			// 2. Create new material instance
			UMaterialInstanceConstant* newMaterialInst = CreateMaterialInstance(assetName, assetPath, TemplateMaterialInst);

			if (!newMaterialInst)
			{
				tmpreateInfo.bInstanceCreated = false;

				continue;
			}


			// 3. Copy Parameter Values
			newMaterialInst->CopyMaterialUniformParametersEditorOnly(TemplateMaterialInst);
			newMaterialInst->PostEditChange();
			newMaterialInst->MarkPackageDirty();

			// 4. Get diffuse texture and set to new material instance
			TArray<UTexture*> textures;
			material->GetUsedTextures(textures, EMaterialQualityLevel::High, true, ERHIFeatureLevel::SM5, true);
			if (textures.Num() > 0)
			{
				tmpreateInfo.bTexCreated = SetTextureParameter(newMaterialInst, TEXT("Albedo"), textures[0]);
			}
			else
			{
				tmpreateInfo.bTexCreated = false;
			}

			tmpreateInfo.bInstanceCreated = true;

			createInfos.Emplace(tmpreateInfo);
		}
	}

	// 5. Show result dialog
	TArray<FString> logInfos;

	int32 instSucceedCount = 0;
	int32 texSucceedCount = 0;

	for(const FMaterialInstanceCreateInfo& info : createInfos)
	{
		logInfos.Emplace(info.PrintInfo());

		instSucceedCount += info.bInstanceCreated ? 1 : 0;
		texSucceedCount += info.bTexCreated ? 1 : 0;
	}

	FString resultInfo = FString::Join(logInfos, TEXT("\n"));

	resultInfo += FString::Printf(TEXT("\n\nTotal MaterialInstance Created: %d, Texture Set: %d"), createInfos.Num(), instSucceedCount, texSucceedCount);

	FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(resultInfo));
}

void UMaterialInstUtility::ModifyMaterialInstanceParameters(UMaterialInstanceModifyData* ModifyData)
{
	if (!ModifyData)
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("ModifyData is nullptr!")));

		return;
	}

	TArray<UObject*> selectedAssets = UEditorUtilityLibrary::GetSelectedAssets();

	for (UObject* obj : selectedAssets)
	{
		if (UMaterialInstanceConstant* materialInst = Cast<UMaterialInstanceConstant>(obj))
		{
			ModifyMaterialInstanceParametersInternal(materialInst, *ModifyData);
		}
	}

	FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Material Instance Parameter modification is completed!")));
}

UMaterialInstanceConstant* UMaterialInstUtility::CreateMaterialInstance(const FString& AssetName, const FString& AssetPath, UMaterialInterface* ParentMaterial)
{
	if (!ParentMaterial)
	{
		return nullptr;
	}

	UMaterialInstanceConstantFactoryNew* Factory = NewObject<UMaterialInstanceConstantFactoryNew>();
	Factory->InitialParent = ParentMaterial;

	FString PackageName = AssetPath + "/" + AssetName;
	PackageName = PackageTools::SanitizePackageName(PackageName);

	UPackage* package = CreatePackage(*PackageName);

	UObject* createdObj = Factory->FactoryCreateNew(UMaterialInstanceConstant::StaticClass(), package, *AssetName, RF_Standalone | RF_Public, nullptr, GWarn);

	UMaterialInstanceConstant* mic = Cast<UMaterialInstanceConstant>(createdObj);

	if (mic)
	{
		FAssetRegistryModule::AssetCreated(mic);

		package->MarkPackageDirty();
	}

	return mic;
}

bool UMaterialInstUtility::SetTextureParameter(UMaterialInstanceConstant* MaterialInst, const FString& ParameterName, UTexture* InTexture)
{
	if (!MaterialInst || !InTexture)
	{
		return false;
	}

	FTextureParameterValue* texParam = MaterialInst->TextureParameterValues.FindByPredicate([&](const FTextureParameterValue& Param)
		{
			return Param.ParameterInfo.Name == FName(*ParameterName);
	});

	if (!texParam)
	{
		return false;
	}

	texParam->ParameterValue = InTexture;

	MaterialInst->PostEditChange();
	MaterialInst->MarkPackageDirty();

	return true;
}

void UMaterialInstUtility::ModifyMaterialInstanceParametersInternal(UMaterialInstanceConstant* MIC, const UMaterialInstanceModifyData& ModifyData)
{
	if (!MIC)
	{
		return;
	}

	// スカラー（浮動小数点）パラメータを変更
	ModifyParameterValues<FScalarParameterValue, float>
	(
		MIC->ScalarParameterValues,
		ModifyData.ScalarParams,
		ModifyData.ExcludeParams,
		[](const FScalarParameterValue& Param, const FName& Name) { return Param.ParameterInfo.Name == Name; },
		[](FScalarParameterValue& Param, const float& Value) { Param.ParameterValue = Value; }
	);

	// テクスチャパラメータを変更
	ModifyParameterValues<FTextureParameterValue, UTexture*>
	(
		MIC->TextureParameterValues,
		ModifyData.TextureParams,
		ModifyData.ExcludeParams,
		[](const FTextureParameterValue& Param, const FName& Name) { return Param.ParameterInfo.Name == Name; },
		[](FTextureParameterValue& Param, UTexture* const& Value) { Param.ParameterValue = Value; }
	);

	// ベクターパラメータを変更
	ModifyParameterValues<FVectorParameterValue, FLinearColor>
	(
		MIC->VectorParameterValues,
		ModifyData.VectorParams,
		ModifyData.ExcludeParams,
		[](const FVectorParameterValue& Param, const FName& Name) { return Param.ParameterInfo.Name == Name; },
		[](FVectorParameterValue& Param, const FLinearColor& Value) { Param.ParameterValue = Value; }
	);

	MIC->PostEditChange();
	MIC->MarkPackageDirty();
}
