#pragma once

// Include Header
#include "UObject/NoExportTypes.h"
#include "Define/MWDefineUI.h"
#include "MWUIManager.generated.h"

// Forward Declare
class UMWRootCanvas;

// Define

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
    virtual void Deinitialize() override {}

    UFUNCTION(BlueprintPure, Category = "Manager", meta = (WorldContext="WorldContext", DisplayName = "GetUIManager"))
	static UMWUIManager* Get(const UObject* WorldContext);

    UMWUIManager();

public:
    /*
     *  Open A widget with the given name.
     * 
     *  @param bFocus : if true, give the focus to the widget.
     */
 	UFUNCTION(BlueprintCallable, Category = "UI")
 	bool OpenUI(const FName& WidgetName, bool bFocus = false);
 
    /*
     * Close a widget that is already display on screen.
     * 
     * @param bForceImmediatedly : set to true to ignore close animation
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

    /* Save the z-order info in types of ui. */
    TMap<EMWUILayer, int32> LayerConfig;
};