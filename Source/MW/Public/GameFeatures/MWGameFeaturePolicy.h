// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameFeatureStateChangeObserver.h"
#include "GameFeaturesProjectPolicies.h"

#include "MWGameFeaturePolicy.generated.h"

class FName;
class UGameFeatureData;
struct FPrimaryAssetId;

/**
 * GameFeature Pluginをメモリへロードして有効化するステートマシンを管理します。
 * ゲーム内蔵Pluginと、Web Serviceなど外部Endpointから通知されたPluginを検出します。
 */
UCLASS(MinimalAPI, Config = Game)
class UMWGameFeaturePolicy : public UDefaultGameFeaturesProjectPolicies
{
	GENERATED_BODY()

public:
	MW_API static UMWGameFeaturePolicy& Get();

	UMWGameFeaturePolicy(const FObjectInitializer& ObjectInitializer);

	//~UGameFeaturesProjectPoliciesインターフェース
	virtual void InitGameFeatureManager() override;
	virtual void ShutdownGameFeatureManager() override;
	virtual TArray<FPrimaryAssetId> GetPreloadAssetListForGameFeature(const UGameFeatureData* GameFeatureToLoad, bool bIncludeLoadedAssets = false) const override;
	virtual bool IsPluginAllowed(const FString& PluginURL, FString* OutReason) const override;
	virtual const TArray<FName> GetPreloadBundleStateForGameFeature() const override;
	virtual void GetGameFeatureLoadingMode(bool& bLoadClientData, bool& bLoadServerData) const override;
	//~UGameFeaturesProjectPoliciesインターフェース終了

private:
	UPROPERTY(Transient)
	TArray<TObjectPtr<UObject>> Observers;
};

// 確認済み
UCLASS()
class UMWGameFeature_HotfixManager : public UObject, public IGameFeatureStateChangeObserver
{
	GENERATED_BODY()

public:
	virtual void OnGameFeatureLoading(const UGameFeatureData* GameFeatureData, const FString& PluginURL) override;
};

// 確認済み
UCLASS()
class UMWGameFeature_AddGameplayCuePaths : public UObject, public IGameFeatureStateChangeObserver
{
	GENERATED_BODY()

public:
	virtual void OnGameFeatureRegistering(const UGameFeatureData* GameFeatureData, const FString& PluginName, const FString& PluginURL) override;
	virtual void OnGameFeatureUnregistering(const UGameFeatureData* GameFeatureData, const FString& PluginName, const FString& PluginURL) override;
};
