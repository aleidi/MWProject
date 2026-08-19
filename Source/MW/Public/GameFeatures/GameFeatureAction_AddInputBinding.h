// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameFeatureAction_WorldActionBase.h"
#include "UObject/SoftObjectPtr.h"
#include "GameFeatureAction_AddInputBinding.generated.h"

class AActor;
class UInputMappingContext;
class UPlayer;
class APlayerController;
struct FComponentRequestHandle;
class UMWInputConfig;

/**
 * LocalPlayerのEnhancedInputシステムへInputMappingContextを追加します。
 * LocalPlayerでEnhancedInputシステムが設定済みであることを前提とします。
 */
UCLASS(MinimalAPI, meta = (DisplayName = "Add Input Binds"))
class UGameFeatureAction_AddInputBinding final : public UGameFeatureAction_WorldActionBase
{
	GENERATED_BODY()

public:
	//~ UGameFeatureActionインターフェース開始
	virtual void OnGameFeatureActivating(FGameFeatureActivatingContext& Context) override;
	virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;
	//~ UGameFeatureActionインターフェース終了

	//~ UObjectインターフェース開始
#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif
	//~ UObjectインターフェース終了

	UPROPERTY(EditAnywhere, Category="Input", meta=(AssetBundles="Client,Server"))
	TArray<TSoftObjectPtr<const UMWInputConfig>> InputConfigs;

private:
	struct FPerContextData
	{
		TArray<TSharedPtr<FComponentRequestHandle>> ExtensionRequestHandles;
		TMap<APawn*, TArray<uint32>> PawnInputBindHandles;
	};

	TMap<FGameFeatureStateChangeContext, FPerContextData> ContextData;

	//~ UGameFeatureAction_WorldActionBaseインターフェース開始
	virtual void AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext) override;
	//~ UGameFeatureAction_WorldActionBaseインターフェース終了

	void Reset(FPerContextData& ActiveData);
	void HandlePawnExtension(AActor* Actor, FName EventName, FGameFeatureStateChangeContext ChangeContext);
	void AddInputMappingForPlayer(APawn* Pawn, FPerContextData& ActiveData);
	void RemoveInputMapping(APawn* Pawn, FPerContextData& ActiveData);
};
