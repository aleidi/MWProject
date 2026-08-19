#pragma once

// ヘッダー
#include "AssetActionUtility.h"
#include "MaterialInstUtility.generated.h"

// 前方宣言
class UMaterialInstanceConstant;
class UMaterialInstance;

// 定義
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
    // MaterialInstance内で変更するスカラー（浮動小数点）パラメータ。
	// MaterialInstance内で変更するスカラー（浮動小数点）パラメータ。
	UPROPERTY(EditDefaultsOnly)
	TMap<FName, float> ScalarParams;

    // MaterialInstance内で変更するテクスチャパラメータ。未指定の場合は無効化（nullを設定）。
	// MaterialInstance内で変更するテクスチャパラメータ。テクスチャを渡さない場合は無効化（nullに設定）を意味します。
	UPROPERTY(EditDefaultsOnly)
	TMap<FName, UTexture*> TextureParams;

    // MaterialInstance内で変更するベクターパラメータ。
	// MaterialInstance内で変更するベクターパラメータ。
	UPROPERTY(EditDefaultsOnly)
	TMap<FName, FLinearColor> VectorParams;

	UPROPERTY(EditDefaultsOnly)
	TSet<FName> ExcludeParams;

    UPROPERTY(EditDefaultsOnly, Category = "StaticSwitch")
	bool bOverrideStaticSwitchParams = false;

    // MaterialInstance内で変更するStatic Switchパラメータ。ExcludeParamsの影響を受けない。
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
    // XSPマテリアルからMaterialInstanceを作成するヘルパー関数。
	// XSPのマテリアルからMaterialInstanceを作成するためのヘルパー関数
	UFUNCTION(CallInEditor, Category = "MaterialInstanceUtility")
    void CreateMaterialInstaFromMaterialXSP(UMaterialInstance* TemplateMaterialInst);

    // FSMマテリアルからMaterialInstanceを作成するヘルパー関数。
    // FSMのマテリアルからMaterialInstanceを作成するためのヘルパー関数
	UFUNCTION(CallInEditor, Category = "MaterialInstanceUtility")
    void CreateMaterialInstaFromMaterialFSM(UMaterialInstance* TemplateMaterialInst);

    // MaterialInstance内のパラメータを変更するヘルパー関数。
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

    // テクスチャはマテリアルプロパティノードへ直接接続する必要がある。
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
        // 除外対象パラメータをスキップ
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
