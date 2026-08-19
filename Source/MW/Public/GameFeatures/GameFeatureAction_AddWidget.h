//// Copyright Epic Games, Inc. All Rights Reserved.
//
//#pragma once
//
//#include "CommonActivatableWidget.h"
//#include "GameFeatureAction_WorldActionBase.h"
////#include "UIExtensionSystem.h"
//
//#include "GameFeatureAction_AddWidget.generated.h"
//
//struct FWorldContext;
//struct FComponentRequestHandle;
//
//USTRUCT()
//struct FMWHUDLayoutRequest
//{
//	GENERATED_BODY()
//
//	// 生成するLayoutWidget
//	UPROPERTY(EditAnywhere, Category=UI, meta=(AssetBundles="Client"))
//	TSoftClassPtr<UCommonActivatableWidget> LayoutClass;
//
//	// Widgetを挿入するLayer
//	UPROPERTY(EditAnywhere, Category=UI, meta=(Categories="UI.Layer"))
//	FGameplayTag LayerID;
//};
//
//
//USTRUCT()
//struct FMWHUDElementEntry
//{
//	GENERATED_BODY()
//
//	// 生成するWidget
//	UPROPERTY(EditAnywhere, Category=UI, meta=(AssetBundles="Client"))
//	TSoftClassPtr<UUserWidget> WidgetClass;
//
//	// Widgetを配置するSlot ID
//	UPROPERTY(EditAnywhere, Category = UI)
//	FGameplayTag SlotID;
//};
//
////////////////////////////////////////////////////////////////////////
//// UGameFeatureAction_AddWidget
//
///**
// * Widgetを追加するGameFeatureActionです。
// */
//UCLASS(MinimalAPI, meta = (DisplayName = "Add Widgets"))
//class UGameFeatureAction_AddWidgets final : public UGameFeatureAction_WorldActionBase
//{
//	GENERATED_BODY()
//
//public:
//	//~ UGameFeatureActionインターフェース開始
//	virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;
//#if WITH_EDITORONLY_DATA
//	virtual void AddAdditionalAssetBundleData(FAssetBundleData& AssetBundleData) override;
//#endif
//	//~ UGameFeatureActionインターフェース終了
//
//	//~ UObjectインターフェース開始
//#if WITH_EDITOR
//	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
//#endif
//	//~ UObjectインターフェース終了
//
//private:
//	// HUDへ追加するLayout
//	UPROPERTY(EditAnywhere, Category=UI, meta=(TitleProperty="{LayerID} -> {LayoutClass}"))
//	TArray<FMWHUDLayoutRequest> Layout;
//
//	// HUDへ追加するWidget
//	UPROPERTY(EditAnywhere, Category=UI, meta=(TitleProperty="{SlotID} -> {WidgetClass}"))
//	TArray<FMWHUDElementEntry> Widgets;
//
//private:
//
//	struct FPerActorData
//	{
//		TArray<TWeakObjectPtr<UCommonActivatableWidget>> LayoutsAdded;
//		TArray<FUIExtensionHandle> ExtensionHandles;
//	};
//
//	struct FPerContextData
//	{
//		TArray<TSharedPtr<FComponentRequestHandle>> ComponentRequests;
//		TMap<FObjectKey, FPerActorData> ActorData; 
//	};
//
//	TMap<FGameFeatureStateChangeContext, FPerContextData> ContextData;
//
//	//~ UGameFeatureAction_WorldActionBaseインターフェース開始
//	virtual void AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext) override;
//	//~ UGameFeatureAction_WorldActionBaseインターフェース終了
//
//	void Reset(FPerContextData& ActiveData);
//
//	void HandleActorExtension(AActor* Actor, FName EventName, FGameFeatureStateChangeContext ChangeContext);
//
//	void AddWidgets(AActor* Actor, FPerContextData& ActiveData);
//	void RemoveWidgets(AActor* Actor, FPerContextData& ActiveData);
//};
