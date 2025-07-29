#pragma once

#include "Engine/DataAsset.h"
#include "MWDefineUI.generated.h"

// Forward Declare
class UMWUserWidget;

/*
 *  Background	: Scene info
 *  Base		: Persistent UI such as character's state, map
 *  Interactive : Inventory, skill, task
 *  Popup		: Confirm window, rewards
 *  Notification: Instant message, prompt, achievement
 *  Topmost		: Very important ui
 *	Debug		: debug ui
 */
UENUM(BlueprintType)
enum class EMWUILayer : uint8
{
	Background,
	Base,
	Interactive,
	Popup,
	Notifiaction,
	Topmost,
	Debug
};

USTRUCT()
struct FMWWidgetLayerInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UUserWidget> Widget;

	UPROPERTY(EditDefaultsOnly)
	EMWUILayer Layer = EMWUILayer::Base;

	/* LayerOffset is added to z-order of Layer. */
	UPROPERTY(EditDefaultsOnly)
	int32 LayerOffset = 0;
};

USTRUCT()
struct FMWUIClassConfig : public FTableRowBase
{
	GENERATED_BODY()

	/* Used in open and close UI. */
	UPROPERTY(EditDefaultsOnly)
	FName Name;
	
	UPROPERTY(EditDefaultsOnly)
	FMWWidgetLayerInfo WidgetConfig;
};