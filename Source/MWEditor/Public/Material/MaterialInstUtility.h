#pragma once

// Include Header
#include "AssetActionUtility.h"
#include "MaterialInstUtility.generated.h"

// Forward Declare
class UMaterialInstanceConstant;
class UMaterialInstance;

// Define
struct FMaterialInstanceCreateInfo
{
	FString MaterialName;

	bool bInstanceCreated = false;

	bool bAlbedoTexSet = false;

    bool bMetallicTexSet = false;

	bool bRoughnessTexSet = false;

    bool bSpecularTexSet = false;

    bool bNormalTexSet = false;

	bool bEmissiveTexSet = false;

	FString PrintInfo() const
	{
		return FString::Printf(TEXT("%s Created[%s], Albedo[%s], Metallic[%s], Roughness[%s], Specular[%s], Normal[%s], Emissive[%s]"),
                *MaterialName,
				bInstanceCreated ? TEXT("O") : TEXT("X"), 
                bAlbedoTexSet ? TEXT("O") : TEXT("X"),
			    bMetallicTexSet ? TEXT("O") : TEXT("X"),
			    bRoughnessTexSet ? TEXT("O") : TEXT("X"),
                bSpecularTexSet ? TEXT("O") : TEXT("X"),
                bNormalTexSet ? TEXT("O") : TEXT("X"),
                bEmissiveTexSet ? TEXT("O") : TEXT("X"));
	}
};

UCLASS()
class UMaterialInstanceModifyData : public UDataAsset
{
	GENERATED_BODY()

public:
	// Scalar Parameters to modify in the MaterialInstance.
	// MaterialInstance内で変更するスカラー（浮動小数点）パラメータ。
	UPROPERTY(EditDefaultsOnly)
	TMap<FName, float> ScalarParams;

	// Texture Parameters to modify in the MaterialInstance. Not passing a texture means to disable (set to null).
	// MaterialInstance内で変更するテクスチャパラメータ。テクスチャを渡さない場合は無効化（nullに設定）を意味します。
	UPROPERTY(EditDefaultsOnly)
	TMap<FName, UTexture*> TextureParams;

	// Vector Parameters to modify in the MaterialInstance.
	// MaterialInstance内で変更するベクターパラメータ。
	UPROPERTY(EditDefaultsOnly)
	TMap<FName, FLinearColor> VectorParams;

	UPROPERTY(EditDefaultsOnly)
	TSet<FName> ExcludeParams;

    UPROPERTY(EditDefaultsOnly, Category = "StaticSwitch")
	bool bOverrideStaticSwitchParams = false;

	// Static Switch Parameters to modify in the MaterialInstance. It's not influenced by ExcludeParams.
	// MaterialInstance内で変更するStatic Switchパラメータ。ExcludeParamsの影響を受けません。
    UPROPERTY(EditDefaultsOnly, Category = "StaticSwitch")
	TMap<FName, bool> StaticSwitchParams;
};

/*
 * @class UMaterialInstUtility
 * 
 * @brief
 *
 * @note
 */
UCLASS()
class UMaterialInstUtility : public UAssetActionUtility
{
	GENERATED_BODY()

public:
	// Helper function to create MaterialInstance from XSP's material.
	// XSPのマテリアルからMaterialInstanceを作成するためのヘルパー関数
	UFUNCTION(CallInEditor, Category = "MaterialInstanceUtility")
    void CreateMaterialInstaFromMaterialXSP(UMaterialInstance* TemplateMaterialInst);

    // Helper function to create MaterialInstance from FSM's material.
    // FSMのマテリアルからMaterialInstanceを作成するためのヘルパー関数
	UFUNCTION(CallInEditor, Category = "MaterialInstanceUtility")
    void CreateMaterialInstaFromMaterialFSM(UMaterialInstance* TemplateMaterialInst);

	// Helper function to modify parameters in the MaterialInstance.
	// MaterialInstance内のパラメータを変更するためのヘルパー関数
	UFUNCTION(CallInEditor, Category = "MaterialInstanceUtility")
	void ModifyMaterialInstanceParameters(UMaterialInstanceModifyData* ModifyData);

private:	
	UMaterialInstanceConstant* CreateMaterialInstance(const FString& AssetName, const FString& AssetPath, UMaterialInterface* ParentMaterial);

	bool SetTextureParameter(UMaterialInstanceConstant* MaterialInst, const FString& ParameterName, UTexture* InTexture);

	void ModifyMaterialInstanceParametersInternal(UMaterialInstanceConstant* MIC, const UMaterialInstanceModifyData& ModifyData);

    template<typename TParamType, typename TValueType>
    void ModifyParameterValues(
        TArray<TParamType>& ParamArray,
        const TMap<FName, TValueType>& ModifyParams,
        const TSet<FName>& ExcludeParams,
        TFunctionRef<bool(const TParamType&, const FName&)> NameMatchFunc,
        TFunctionRef<void(TParamType&, const TValueType&)> SetValueFunc);

	// The texture must be directly connected to the material property node.
	// テクスチャはマテリアルプロパティノードに直接接続されている必要があります。
    UTexture* GetTextureFromMaterialProperty(UMaterial* Material, EMaterialProperty Property);
};

template<typename TParamType, typename TValueType>
void UMaterialInstUtility::ModifyParameterValues(
    TArray<TParamType>& ParamArray,
    const TMap<FName, TValueType>& ModifyParams,
    const TSet<FName>& ExcludeParams,
    TFunctionRef<bool(const TParamType&, const FName&)> NameMatchFunc,
    TFunctionRef<void(TParamType&, const TValueType&)> SetValueFunc)
{
    for (int32 i = ParamArray.Num() - 1; i >= 0; --i)
    {
        const TParamType& param = ParamArray[i];

        bool bIsExcluded = ExcludeParams.Contains(param.ParameterInfo.Name);

        bool bExists = false;

        for (const auto& NewParam : ModifyParams)
        {
            if (NameMatchFunc(param, NewParam.Key))
            {
                bExists = true;
                break;
            }
        }

        if (!bExists && !bIsExcluded)
        {
            ParamArray.RemoveAt(i);
        }
    }

    for (const auto& modifyParam : ModifyParams)
    {
		// skip excluded params
        // 除外されたパラメータをスキップする
        if (ExcludeParams.Contains(modifyParam.Key))
        {
            continue;
        }

        TParamType* foundParam = nullptr;

        for (TParamType& param : ParamArray)
        {
            if (NameMatchFunc(param, modifyParam.Key))
            {
                foundParam = &param;
                break;
            }
        }

        if (foundParam)
        {
            SetValueFunc(*foundParam, modifyParam.Value);
        }
    }
}
