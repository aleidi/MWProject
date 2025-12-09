#pragma once

#include "Engine/DataAsset.h"
#include "MWDefineUI.generated.h"

// Forward Declare
class UMWUserWidget;

/*
 *  Background	: Scene info
 *  Base		: Persistent UI such as character's state, map
 *  Battle		: Damage numbers, combo indicators, skill cooldown, enemy health bars
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
	Battle,
	Interactive,
	Popup,
	Notification,
	Topmost,
	Debug
};

UENUM(BlueprintType)
enum class EMWSceneUIType : uint8 
{
	None = 0,
	LoginUI,
	CharacterCustomize_MainUI,
	BaseUI,
};

UENUM(BlueprintType)
enum class EMWWindowUIType : uint8 
{
	None = 0,	
	LoginUI,
	//InGameManagedUI,
	DialogueUI,
	LoadingUI,
	InventoryUI,
	Debug_PartsSelctUI,
	DyeingUI,
	ChatUI,
	PacketMonitorUI,
	TransporterOarControlUI,
	TransporterAirBalloonControlUI,
	CommercePartyAirCombatUI,
	DialogO,
	DialogU,
	ComposeUI,
	Debug_InstrumentTestUI,
	ContextMenuUI,
	ItemContextMenuUI,
	ClockUI,
	Debug_GameTimeUI,
	Debug_SoundTestUI,
	CharacterCustomize_MainUI,
	BagInventoryUI,
	SkillListUI,
	Quest_ListPopupUI,
	Quest_DetailPopupUI,
	CharcterInfoUI,
	ErrorPacketNoticeUI,
	SkillLevelUpUI,
	Respawn_MainUI,
	FloatingTabUI,
	MyLevelUpUI,
	SkillDetailPopupUI,
	Debug_GameServerTimeUI,
	APLevelupPopupUI,
	OptionUI,
	OptionContentUI,
	Common_ItemSelectPopupUI,
	Common_BundlePopupUI,
	CheatUI,
	GameLogoffUI,
	GameLogoutUI,
	RecipeManualUI,
	RecipeSelectionUI,
	RecipeCompleteManualSelectionUI,
	Debug_ActionEffectTestUI,
	RecipeProductionUI,
	RecipeProductionMakingUI,

	//////////  Debug Or Temp
	InventoryRecycleBinUI,
};

UENUM()
enum class EMWPopupUIType
{
	Common_Message_PopupUI,
	APconsume_PopupUI,
	ConfirmDeletePopupUI
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