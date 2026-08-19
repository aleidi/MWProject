#pragma once

#include "Engine/DataAsset.h"
#include "MWDefineUI.generated.h"

// 前方宣言
class UMWUserWidget;

/*
 *  Background	: シーン情報
 *  Base		: キャラクター状態やMapなどの常設UI
 *  Battle		: ダメージ値、コンボ表示、スキルクールダウン、敵HPバー
 *  Interactive	: インベントリ、スキル、タスク
 *  Popup		: 確認画面、報酬
 *  Notification: メッセージ、プロンプト、実績通知
 *  Topmost		: 最優先UI
 *	Debug		: デバッグUI
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

	//////////  デバッグ／一時利用
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

	/* LayerOffsetをLayerのZ-orderに加算します。 */
	UPROPERTY(EditDefaultsOnly)
	int32 LayerOffset = 0;
};

USTRUCT()
struct FMWUIClassConfig : public FTableRowBase
{
	GENERATED_BODY()

	/* UIの開閉に使用します。 */
	UPROPERTY(EditDefaultsOnly)
	FName Name;
	
	UPROPERTY(EditDefaultsOnly)
	FMWWidgetLayerInfo WidgetConfig;
};