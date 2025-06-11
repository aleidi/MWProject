#pragma once

// Include Header
#include "Subsystems/GameInstanceSubsystem.h"
#include "Define/MWDefineUI.h"
#include "MWUIManager.generated.h"

// Forward Declare
class UMWRootCanvas;

// Macro

/*
 * @class UMWUIManager
 * 
 * @brief
 *
 * @note
 */
UCLASS()
class UMWUIManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
    /** Implement this for initialization of instances of the system */
    virtual void Initialize(FSubsystemCollectionBase& Collection);

    /** Implement this for deinitialization of instances of the system */
    virtual void Deinitialize();

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