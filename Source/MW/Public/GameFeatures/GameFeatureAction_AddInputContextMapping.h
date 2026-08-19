// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameFeatureAction_WorldActionBase.h"
#include "UObject/SoftObjectPtr.h"
#include "GameFeatureAction_AddInputContextMapping.generated.h"

class AActor;
class UInputMappingContext;
class UPlayer;
class APlayerController;
struct FComponentRequestHandle;

USTRUCT()
struct FInputMappingContextAndPriority
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="Input", meta=(AssetBundles="Client,Server"))
	TSoftObjectPtr<UInputMappingContext> InputMapping;

	// 優先度の高いInputMappingを優先します。
	UPROPERTY(EditAnywhere, Category="Input")
	int32 Priority = 0;
	
	/** trueの場合、このGameFeatureActionの登録時にMappingContextを設定へ登録します。 */
	UPROPERTY(EditAnywhere, Category="Input")
	bool bRegisterWithSettings = true;
};

/**
 * LocalPlayerのEnhancedInputシステムへInputMappingContextを追加します。
 * LocalPlayerでEnhancedInputシステムが設定済みであることを前提とします。
 */
UCLASS(MinimalAPI, meta = (DisplayName = "Add Input Mapping"))
class UGameFeatureAction_AddInputContextMapping final : public UGameFeatureAction_WorldActionBase
{
	GENERATED_BODY()

public:
	//~UGameFeatureActionインターフェース
	virtual void OnGameFeatureRegistering() override;
	virtual void OnGameFeatureActivating(FGameFeatureActivatingContext& Context) override;
	virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;
	virtual void OnGameFeatureUnregistering() override;
	//~UGameFeatureActionインターフェース終了

	//~UObjectインターフェース
#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif
	//~UObjectインターフェース終了

	UPROPERTY(EditAnywhere, Category="Input")
	TArray<FInputMappingContextAndPriority> InputMappings;

private:
	struct FPerContextData
	{
		TArray<TSharedPtr<FComponentRequestHandle>> ExtensionRequestHandles;
		TArray<TWeakObjectPtr<APlayerController>> ControllersAddedTo;
	};

	TMap<FGameFeatureStateChangeContext, FPerContextData> ContextData;
	
	/** IMC登録のためGameInstance変更時に呼び出すDelegate。 */
	FDelegateHandle RegisterInputContextMappingsForGameInstanceHandle;

	/** 所有するInputMappingContextをInputRegistrySubsystemへ登録し、GameInstance開始およびLocalPlayer追加／削除へバインドします。 */
	void RegisterInputMappingContexts();
	
	/** 指定GameInstance向けにInputMappingContextをInputRegistrySubsystemへ登録します。GameInstance開始時にも呼び出されます。 */
	void RegisterInputContextMappingsForGameInstance(UGameInstance* GameInstance);

	/** 指定LocalPlayer向けにInputMappingContextをInputRegistrySubsystemへ登録します。LocalPlayer追加時にも呼び出されます。 */
	void RegisterInputMappingContextsForLocalPlayer(ULocalPlayer* LocalPlayer);

	/** InputMappingContextをInputRegistrySubsystemから登録解除し、GameInstance開始およびLocalPlayer追加／削除のバインドを解除します。 */
	void UnregisterInputMappingContexts();

	/** 指定GameInstance向けのInputMappingContextをInputRegistrySubsystemから登録解除します。 */
	void UnregisterInputContextMappingsForGameInstance(UGameInstance* GameInstance);

	/** 指定LocalPlayer向けのInputMappingContextをInputRegistrySubsystemから登録解除します。LocalPlayer削除時にも呼び出されます。 */
	void UnregisterInputMappingContextsForLocalPlayer(ULocalPlayer* LocalPlayer);

	//~UGameFeatureAction_WorldActionBaseインターフェース
	virtual void AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext) override;
	//~UGameFeatureAction_WorldActionBaseインターフェース終了

	void Reset(FPerContextData& ActiveData);
	void HandleControllerExtension(AActor* Actor, FName EventName, FGameFeatureStateChangeContext ChangeContext);
	void AddInputMappingForPlayer(UPlayer* Player, FPerContextData& ActiveData);
	void RemoveInputMapping(APlayerController* PlayerController, FPerContextData& ActiveData);
};
