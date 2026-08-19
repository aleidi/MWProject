#pragma once

// ヘッダーをインクルード
#include "Define/MWDefineUI.h"
#include "Interface/MWManagerInterface.h"
#include "UObject/NoExportTypes.h"
#include "MWUIManager.generated.h"

// 前方宣言
class UMWRootCanvas;

// 定義

/*
 * @class UMWUIManager
 * 
 * @brief
 *
 * @note
 */
UCLASS()
class MW_API UMWUIManager : public UObject, public IMWManagerInterface
{
	GENERATED_BODY()

public:
    virtual void Initialize() override;
    virtual void Deinitialize() override;

    UFUNCTION(BlueprintPure, Category = "Manager", meta = (WorldContext="WorldContext", DisplayName = "GetUIManager"))
	static UMWUIManager* Get(const UObject* WorldContext);

    UMWUIManager();

public:
    /*
     * 指定した名前のWidgetを開きます。
     * 
     * @param bFocus trueの場合はWidgetへFocusを設定します。
     */
 	UFUNCTION(BlueprintCallable, Category = "UI")
 	bool OpenUI(const FName& WidgetName, bool bFocus = false);
 
    /*
     * 画面に表示中のWidgetを閉じます。
     * 
     * @param bForceImmediatedly trueの場合はCloseAnimationを省略します。
     */
 	UFUNCTION(BlueprintCallable, Category = "UI")
 	bool CloseUI(const FName& WidgetName, bool bForceImmediately = false);

private:
    void OnGameModeBeginPlay(AGameModeBase* NewGameMode);

    bool HasRootCanvas();

    bool CreateCanvas();

    void LoadDataConfig();

private:
    UPROPERTY()
	TMap<FName, FMWWidgetLayerInfo> WidgetInfoMap;

    UPROPERTY()
    TMap<FName, TObjectPtr<UUserWidget>> CachedWidgetInst;

    UPROPERTY()
    TObjectPtr<UMWRootCanvas> RootCanvas;

    /* UI種別ごとのZ-order情報を保持します。 */
    TMap<EMWUILayer, int32> LayerConfig;

	FDelegateHandle OnGameModeBeginPlayHandle;
};
