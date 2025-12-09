#include "Gameplay/Battle/MWActionExecutor.h"
#include "Data/MWMasterData.h"
#include "Input/MWInputConfig.h"
#include "Input/MWInputUtility.h"
#include "Kismet/GameplayStatics.h"
#include "MWLogChannels.h"
#include "System/MWAssetManager.h"

UE_DISABLE_OPTIMIZATION

// Console variable to control LogPhaseInfo output
static TAutoConsoleVariable<int32> CVarLogPhaseInfo(
    TEXT("MW.Battle.LogPhaseInfo"),
    2,
    TEXT("Controls LogPhaseInfo logging output.\n")
    TEXT("0: Disabled\n")
    TEXT("1: Enabled (Log only)\n")
    TEXT("2: Enabled (Log + On-Screen Debug)"),
    ECVF_Default
);

#define CHANGINGAVATAR_DURATION 3.f
#define USINGITEM_DURATION 3.f
#define USINGSPIRIT_DURATION 3.f

bool FMWPlayerActionContext::IsInputTimeout() const
{
    return (TotalTime - LastInputTime) > InputTimeoutDuration;
}

void FMWPlayerActionContext::RecordInput()
{
    LastInputTime = TotalTime;
}

void FMWPlayerActionContext::RecordEnterPhaseTime()
{
    EnterPhaseTime = TotalTime;
}

float FMWPlayerActionContext::GetTimeSinceEnterPhase() const
{
    return TotalTime - EnterPhaseTime;
}

bool FMWPlayerActionContext::CanCancelAction() const
{
    return bCanCancelCurrentAction;
}

void FMWPlayerActionContext::Reset()
{
    CurrentActionType = EPlayerActionType::None;
    LastActionType = EPlayerActionType::None;
    AttackTarget = nullptr;
    LastInputTime = 0.f;
    bCanCancelCurrentAction = false;
}

void UMWActionExecutor::SetActionUnits(const FMWTeam& InPlayerTeam, const FMWTeam& InEnemyTeam)
{
    PlayerTeam = InPlayerTeam;
    EnemyTeam = InEnemyTeam;
}

void UMWEnemyActionExecutor::Init()
{
    // TODO : init action points of align dynamically
    GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("Enemy Turn"));
}

void UMWEnemyActionExecutor::Update(float DeltaTime, const FMWActionExecutorData& InInfo, bool& OutIsComplete)
{
	OutIsComplete = false;

    time += DeltaTime;
    if (time > 2.f)
    {
        OutIsComplete = true;
    }
}

void UMWPlayerActionExecutor::Init()
{
    InitActionPhase();

	DisplayUI();

	SetupInput();

    GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("Player Turn"));

    // TODO : init action points of align dynamically
}

void UMWPlayerActionExecutor::Uninit()
{
    UMWInputUtility::ClearBindingsForObject(this);
}

void UMWPlayerActionExecutor::Update(float DeltaTime, const FMWActionExecutorData& InInfo, bool& OutIsComplete)
{
    UpdateActionPhase(DeltaTime);

	OutIsComplete = bActionCompleted;

    //OutIsComplete = false;

    //time += DeltaTime;
    //if (time > 2.f)
    //{
    //    OutIsComplete = true;
    //}
	// action points all used
	// enemy all died
}

bool UMWPlayerActionExecutor::TryCancelCurrentAction()
{
    if(!CanCancelAction())
    {
        return false;
	}

    switch (CurrentPhase)
    {
        case EPlayerActionPhase::AttackPreparing:
        case EPlayerActionPhase::SelectingAvatar:
        case EPlayerActionPhase::SelectingItem:
        case EPlayerActionPhase::SelectingSpirit:
			ReturnToCommandSelection();
			return true;

		case EPlayerActionPhase::SelectingItemTarget:
            ChangePhase(EPlayerActionPhase::SelectingItem);
            return true;

		case EPlayerActionPhase::SelectingSpiritTarget:
            ChangePhase(EPlayerActionPhase::SelectingSpirit);
            return true;
    }

    return false;
}

bool UMWPlayerActionExecutor::CanCancelAction() const
{
    return ActionContext.CanCancelAction();
}

void UMWPlayerActionExecutor::UpdateActionPhase(float DeltaTime)
{
    ActionContext.TotalTime += DeltaTime;

    switch (CurrentPhase)
    {
		case EPlayerActionPhase::Idle: UpdatePhase_Idle(DeltaTime); break;
		case EPlayerActionPhase::WaitingCommand: UpdatePhase_WaitingCommand(DeltaTime); break;
		case EPlayerActionPhase::AttackPreparing: UpdatePhase_AttackPreparing(DeltaTime); break;
		case EPlayerActionPhase::AttackExecuting: UpdatePhase_AttackExecuting(DeltaTime); break;
		case EPlayerActionPhase::SelectingAvatar: UpdatePhase_SelectingAvatar(DeltaTime); break;
		case EPlayerActionPhase::ChangingAvatar: UpdatePhase_ChangingAvatar(DeltaTime); break;
		case EPlayerActionPhase::SelectingItem: UpdatePhase_SelectingItem(DeltaTime); break;
		case EPlayerActionPhase::SelectingItemTarget: UpdatePhase_SelectingItemTarget(DeltaTime); break;
		case EPlayerActionPhase::UsingItem: UpdatePhase_UsingItem(DeltaTime); break;
		case EPlayerActionPhase::SelectingSpirit: UpdatePhase_SelectingSpirit(DeltaTime); break;
		case EPlayerActionPhase::SelectingSpiritTarget: UpdatePhase_SelectingSpiritTarget(DeltaTime); break;
		case EPlayerActionPhase::UsingSpirit: UpdatePhase_UsingSpirit(DeltaTime); break;
		case EPlayerActionPhase::Escaping: UpdatePhase_Escaping(DeltaTime); break;
		case EPlayerActionPhase::Returning: UpdatePhase_Returning(DeltaTime); break;
		case EPlayerActionPhase::TurnComplete: UpdatePhase_TurnComplete(DeltaTime); break;
    }
}

void UMWPlayerActionExecutor::ChangePhase(EPlayerActionPhase NewPhase)
{
    if (CurrentPhase == NewPhase)
    {
        return;
    }

    OnExitPhase(CurrentPhase);

	LogPhaseInfo(FString::Printf(TEXT("Phase changed: %s -> %s"),
		*UEnum::GetValueAsString(CurrentPhase),
		*UEnum::GetValueAsString(NewPhase)
	));

    CurrentPhase = NewPhase;


    OnEnterPhase(NewPhase);
}

void UMWPlayerActionExecutor::OnEnterPhase(EPlayerActionPhase Phase)
{
    switch (Phase)
    {
        case EPlayerActionPhase::Idle: OnEnterPhase_Idle(); break;
		case EPlayerActionPhase::WaitingCommand: OnEnterPhase_WaitingCommand(); break;
		case EPlayerActionPhase::AttackPreparing: OnEnterPhase_AttackPreparing(); break;
		case EPlayerActionPhase::AttackExecuting: OnEnterPhase_AttackExecuting(); break;
		case EPlayerActionPhase::SelectingAvatar: OnEnterPhase_SelectingAvatar(); break;
		case EPlayerActionPhase::ChangingAvatar: OnEnterPhase_ChangingAvatar(); break;
		case EPlayerActionPhase::SelectingItem: OnEnterPhase_SelectingItem(); break;
		case EPlayerActionPhase::SelectingItemTarget: OnEnterPhase_SelectingItemTarget(); break;
		case EPlayerActionPhase::UsingItem: OnEnterPhase_UsingItem(); break;
		case EPlayerActionPhase::SelectingSpirit: OnEnterPhase_SelectingSpirit(); break;
		case EPlayerActionPhase::SelectingSpiritTarget: OnEnterPhase_SelectingSpiritTarget(); break;
		case EPlayerActionPhase::UsingSpirit: OnEnterPhase_UsingSpirit(); break;
		case EPlayerActionPhase::Escaping: OnEnterPhase_Escaping(); break;
		case EPlayerActionPhase::Returning: OnEnterPhase_Returning(); break;
		case EPlayerActionPhase::TurnComplete: OnEnterPhase_TurnComplete(); break;
    }
}

void UMWPlayerActionExecutor::OnExitPhase(EPlayerActionPhase Phase)
{
    switch (Phase)
    {
		case EPlayerActionPhase::Idle: OnExitPhase_Idle(); break;
		case EPlayerActionPhase::WaitingCommand: OnExitPhase_WaitingCommand(); break;
		case EPlayerActionPhase::AttackPreparing: OnExitPhase_AttackPreparing(); break;
		case EPlayerActionPhase::AttackExecuting: OnExitPhase_AttackExecuting(); break;
		case EPlayerActionPhase::SelectingAvatar: OnExitPhase_SelectingAvatar(); break;
		case EPlayerActionPhase::ChangingAvatar: OnExitPhase_ChangingAvatar(); break;
		case EPlayerActionPhase::SelectingItem: OnExitPhase_SelectingItem(); break;
		case EPlayerActionPhase::SelectingItemTarget: OnExitPhase_SelectingItemTarget(); break;
		case EPlayerActionPhase::UsingItem: OnExitPhase_UsingItem(); break;
		case EPlayerActionPhase::SelectingSpirit: OnExitPhase_SelectingSpirit(); break;
		case EPlayerActionPhase::SelectingSpiritTarget: OnExitPhase_SelectingSpiritTarget(); break;
		case EPlayerActionPhase::UsingSpirit: OnExitPhase_UsingSpirit(); break;
		case EPlayerActionPhase::Escaping: OnExitPhase_Escaping(); break;
		case EPlayerActionPhase::Returning: OnExitPhase_Returning(); break;
		case EPlayerActionPhase::TurnComplete: OnExitPhase_TurnComplete(); break;
    }
}

void UMWPlayerActionExecutor::InitActionPhase()
{
    CurrentPhase = EPlayerActionPhase::Idle;
    ActionContext.TotalTime = 0.f;
    ActionContext.RecordInput();
}

void UMWPlayerActionExecutor::ReturnToCommandSelection()
{
    ActionContext.AttackTarget = nullptr;
	ActionContext.LastActionType = EPlayerActionType::None;
	ActionContext.CurrentActionType = EPlayerActionType::None;

    ChangePhase(EPlayerActionPhase::WaitingCommand);
}

void UMWPlayerActionExecutor::OnEnterPhase_Idle()
{
    LogPhaseInfo(TEXT("Enter Idle"));
}

void UMWPlayerActionExecutor::OnEnterPhase_WaitingCommand()
{
    LogPhaseInfo(FString::Printf(TEXT("Enter WaitingCommand - AP: %d"), ActionContext.ActionPoints));

    ActionContext.bCanCancelCurrentAction = false;

	// Reset action type
	// アクションタイプをリセット
	ActionContext.CurrentActionType = EPlayerActionType::None;

    UMWInputUtility::EnableMappingContext(UGameplayStatics::GetPlayerController(this, 0), MWGameplayTags::IMC_BattleCommand);
}

void UMWPlayerActionExecutor::OnEnterPhase_AttackPreparing()
{
    LogPhaseInfo(TEXT("Enter AttackPreparing (选择攻击目标)"));

    ActionContext.CurrentActionType = EPlayerActionType::Attack;
    ActionContext.bCanCancelCurrentAction = true;

    UMWInputUtility::EnableMappingContext(UGameplayStatics::GetPlayerController(this, 0), MWGameplayTags::IMC_BattleCommand_Attack);
}


void UMWPlayerActionExecutor::OnEnterPhase_AttackExecuting()
{
    LogPhaseInfo(TEXT("Enter AttackExecuting (不可取消)"));

    ActionContext.bCanCancelCurrentAction = false;

	// TODO : Execute attack logic
}

void UMWPlayerActionExecutor::OnEnterPhase_SelectingAvatar()
{
    LogPhaseInfo(TEXT("Enter SelectingAvatar"));

    ActionContext.CurrentActionType = EPlayerActionType::ChangeAvatar;
    ActionContext.bCanCancelCurrentAction = true;

    UMWInputUtility::EnableMappingContext(UGameplayStatics::GetPlayerController(this, 0), MWGameplayTags::IMC_BattleCommand_ChangeAvatar);
}

void UMWPlayerActionExecutor::OnEnterPhase_ChangingAvatar()
{
    LogPhaseInfo(TEXT("Enter ChangingAvatar"));

    ActionContext.bCanCancelCurrentAction = false;
}

void UMWPlayerActionExecutor::OnEnterPhase_SelectingItem()
{
    LogPhaseInfo(TEXT("Enter SelectingItem"));

    ActionContext.CurrentActionType = EPlayerActionType::UseItem;
    ActionContext.bCanCancelCurrentAction = true;

	// TODO : Display item selection UI

    UMWInputUtility::EnableMappingContext(UGameplayStatics::GetPlayerController(this, 0), MWGameplayTags::IMC_BattleCommand_UseItem);
}

void UMWPlayerActionExecutor::OnEnterPhase_SelectingItemTarget()
{
    LogPhaseInfo(TEXT("Enter SelectingItemTarget"));

    ActionContext.bCanCancelCurrentAction = true;

	// TODO : Display item target selection UI
}

void UMWPlayerActionExecutor::OnEnterPhase_UsingItem()
{
    LogPhaseInfo(TEXT("Enter UsingItem"));

    ActionContext.bCanCancelCurrentAction = false;

	// TODO : Execute item usage logic
}

void UMWPlayerActionExecutor::OnEnterPhase_SelectingSpirit()
{
    LogPhaseInfo(TEXT("Enter SelectingSpirit"));

    ActionContext.CurrentActionType = EPlayerActionType::UseSpirit;
    ActionContext.bCanCancelCurrentAction = true;

	// TODO : Display spirit skill selection UI

    UMWInputUtility::EnableMappingContext(UGameplayStatics::GetPlayerController(this, 0), MWGameplayTags::IMC_BattleCommand_UseSpirit);
}

void UMWPlayerActionExecutor::OnEnterPhase_SelectingSpiritTarget()
{
    LogPhaseInfo(TEXT("Enter SelectingSpiritTarget"));

    ActionContext.bCanCancelCurrentAction = true;

	// TODO : Display skill range and selected target
}

void UMWPlayerActionExecutor::OnEnterPhase_UsingSpirit()
{
    LogPhaseInfo(TEXT("Enter UsingSpirit"));

    ActionContext.bCanCancelCurrentAction = false;

	// TODO : Execute spirit skill logic
}

void UMWPlayerActionExecutor::OnEnterPhase_Escaping()
{
    LogPhaseInfo(TEXT("Enter Escaping"));

    ActionContext.bCanCancelCurrentAction = false;

	// TODO : Execute escape logic
}

void UMWPlayerActionExecutor::OnEnterPhase_Returning()
{
    LogPhaseInfo(TEXT("Enter Returning"));

    ActionContext.bCanCancelCurrentAction = false;

    // TODO : Disable All input

	// TODO : Execute return logic
}

void UMWPlayerActionExecutor::OnEnterPhase_TurnComplete()
{
    LogPhaseInfo(TEXT("Enter TurnComplete"));

    bActionCompleted = true;

    ActionContext.Reset();
}

void UMWPlayerActionExecutor::OnExitPhase_Idle()
{
    LogPhaseInfo(TEXT("Exit Idle"));
}

void UMWPlayerActionExecutor::OnExitPhase_WaitingCommand()
{
    LogPhaseInfo(TEXT("Exit WaitingCommand"));

    UMWInputUtility::DisableMappingContext(UGameplayStatics::GetPlayerController(this, 0), MWGameplayTags::IMC_BattleCommand);
}

void UMWPlayerActionExecutor::OnExitPhase_AttackPreparing()
{
    LogPhaseInfo(TEXT("Exit AttackPreparing"));
}

void UMWPlayerActionExecutor::OnExitPhase_AttackExecuting()
{
    LogPhaseInfo(TEXT("Exit AttackExecuting"));

    UMWInputUtility::DisableMappingContext(UGameplayStatics::GetPlayerController(this, 0), MWGameplayTags::IMC_BattleCommand_Attack);
}

void UMWPlayerActionExecutor::OnExitPhase_SelectingAvatar()
{
    LogPhaseInfo(TEXT("Exit SelectingAvatar"));

    UMWInputUtility::DisableMappingContext(UGameplayStatics::GetPlayerController(this, 0), MWGameplayTags::IMC_BattleCommand_ChangeAvatar);
}

void UMWPlayerActionExecutor::OnExitPhase_ChangingAvatar()
{
    LogPhaseInfo(TEXT("Exit ChangingAvatar"));
}

void UMWPlayerActionExecutor::OnExitPhase_SelectingItem()
{
	LogPhaseInfo(TEXT("Exit SelectingItem"));
}

void UMWPlayerActionExecutor::OnExitPhase_SelectingItemTarget()
{
    LogPhaseInfo(TEXT("Exit SelectingItemTarget"));
}

void UMWPlayerActionExecutor::OnExitPhase_UsingItem()
{
    LogPhaseInfo(TEXT("Exit UsingItem"));

    UMWInputUtility::DisableMappingContext(UGameplayStatics::GetPlayerController(this, 0), MWGameplayTags::IMC_BattleCommand_UseItem);
}

void UMWPlayerActionExecutor::OnExitPhase_SelectingSpirit()
{
    LogPhaseInfo(TEXT("Exit SelectingSpirit"));
}

void UMWPlayerActionExecutor::OnExitPhase_SelectingSpiritTarget()
{
    LogPhaseInfo(TEXT("Exit SelectingSpiritTarget"));
}

void UMWPlayerActionExecutor::OnExitPhase_UsingSpirit()
{
	LogPhaseInfo(TEXT("Exit UsingSpirit"));

    UMWInputUtility::DisableMappingContext(UGameplayStatics::GetPlayerController(this, 0), MWGameplayTags::IMC_BattleCommand_UseSpirit);
}

void UMWPlayerActionExecutor::OnExitPhase_Escaping()
{
    LogPhaseInfo(TEXT("Exit Escaping"));

	// TODO : Clean up escape-related resources
}

void UMWPlayerActionExecutor::OnExitPhase_Returning()
{
    LogPhaseInfo(TEXT("Exit Returning"));

	// TODO : Ensure all character is exactly back to start point
}

void UMWPlayerActionExecutor::OnExitPhase_TurnComplete()
{
    LogPhaseInfo(TEXT("Exit TurnComplete"));
}

void UMWPlayerActionExecutor::UpdatePhase_Idle(float DeltaTime)
{
    ChangePhase(EPlayerActionPhase::WaitingCommand);
}

void UMWPlayerActionExecutor::UpdatePhase_WaitingCommand(float DeltaTime)
{
	// State change is trigger by input events
}

void UMWPlayerActionExecutor::UpdatePhase_AttackPreparing(float DeltaTime)
{
    // State change is trigger by input events
	// Cancel -> WaitingCommand
	// Attack -> AttackExecuting
}

void UMWPlayerActionExecutor::UpdatePhase_AttackExecuting(float DeltaTime)
{
    // TODO : Check if action is completed
	// Complete Condition : 1.AP consumed out 2.No input for X seconds from last input
    
    // temp test code
    if(ActionContext.GetTimeSinceEnterPhase() > 2.f)
    {
        ChangePhase(EPlayerActionPhase::Returning);
	}
}

void UMWPlayerActionExecutor::UpdatePhase_SelectingAvatar(float DeltaTime)
{
    // State change is trigger by input events
}

void UMWPlayerActionExecutor::UpdatePhase_ChangingAvatar(float DeltaTime)
{
    // State change is trigger by input events
    if(ActionContext.GetTimeSinceEnterPhase() > CHANGINGAVATAR_DURATION)
    {
        ChangePhase(EPlayerActionPhase::SelectingAvatar);
	}
}

void UMWPlayerActionExecutor::UpdatePhase_SelectingItem(float DeltaTime)
{
    // State change is trigger by input events

}

void UMWPlayerActionExecutor::UpdatePhase_SelectingItemTarget(float DeltaTime)
{
    // State change is trigger by input events

}

void UMWPlayerActionExecutor::UpdatePhase_UsingItem(float DeltaTime)
{
    // State change is trigger by input events
    if (ActionContext.GetTimeSinceEnterPhase() > USINGITEM_DURATION)
    {
        ChangePhase(EPlayerActionPhase::SelectingItem);
    }
}

void UMWPlayerActionExecutor::UpdatePhase_SelectingSpirit(float DeltaTime)
{
    // State change is trigger by input events

}

void UMWPlayerActionExecutor::UpdatePhase_SelectingSpiritTarget(float DeltaTime)
{
    // State change is trigger by input events

}

void UMWPlayerActionExecutor::UpdatePhase_UsingSpirit(float DeltaTime)
{
    // State change is trigger by input events
    if (ActionContext.GetTimeSinceEnterPhase() > USINGSPIRIT_DURATION)
    {
        ChangePhase(EPlayerActionPhase::SelectingAvatar);
    }
}

void UMWPlayerActionExecutor::UpdatePhase_Escaping(float DeltaTime)
{
    // TODO : Check if escape is completed
	// State change is automatic or trigger by escape completion

    // temp code
    if (ActionContext.GetTimeSinceEnterPhase() > 2.f)
    {
        ChangePhase(EPlayerActionPhase::TurnComplete);
    }
}

void UMWPlayerActionExecutor::UpdatePhase_Returning(float DeltaTime)
{
	// check whether characters are returned to start point

    // temp code
    if (ActionContext.GetTimeSinceEnterPhase() > 2.f)
    {
		ChangePhase(EPlayerActionPhase::TurnComplete);
    }
}

void UMWPlayerActionExecutor::UpdatePhase_TurnComplete(float DeltaTime)
{

}

void UMWPlayerActionExecutor::LogPhaseInfo(const FString& Message, bool bIsWarning) const
{
    const int32 logLevel = CVarLogPhaseInfo.GetValueOnGameThread();
    
    if (logLevel <= 0)
    {
        return; // Logging disabled
    }

    const FString FullMessage = FString::Printf(
        TEXT("[PlayerActionExecutor][%s] %s"),
        *UEnum::GetValueAsString(CurrentPhase),
        *Message
    );

    if (bIsWarning)
    {
        UE_LOG(LogMWBattle, Warning, TEXT("%s"), *FullMessage);
    }
    else
    {
        UE_LOG(LogMWBattle, Log, TEXT("%s"), *FullMessage);
    }

#if !UE_BUILD_SHIPPING
    if (logLevel >= 2 && GEngine)
    {
        GEngine->AddOnScreenDebugMessage(
            -1,
            2.f,
            bIsWarning ? FColor::Orange : FColor::Green,
            FullMessage
        );
    }
#endif
}

void UMWPlayerActionExecutor::DisplayUI()
{

}

void UMWPlayerActionExecutor::SetupInput()
{
    // ==== Base Commands ====
    UMWInputUtility::BindInputAction(MWGameplayTags::IMC_BattleCommand, MWGameplayTags::IATag_BC_Attack, ETriggerEvent::Triggered, this, &ThisClass::OnCmdAttack);
	UMWInputUtility::BindInputAction(MWGameplayTags::IMC_BattleCommand, MWGameplayTags::IATag_BC_ChangeAvatar, ETriggerEvent::Triggered, this, &ThisClass::OnCmdChangeAvatar);
	UMWInputUtility::BindInputAction(MWGameplayTags::IMC_BattleCommand, MWGameplayTags::IATag_BC_UseItem, ETriggerEvent::Triggered, this, &ThisClass::OnCmdUseItem);
	UMWInputUtility::BindInputAction(MWGameplayTags::IMC_BattleCommand, MWGameplayTags::IATag_BC_UseSpirit, ETriggerEvent::Triggered, this, &ThisClass::OnCmdUseSpirit);
	UMWInputUtility::BindInputAction(MWGameplayTags::IMC_BattleCommand, MWGameplayTags::IATag_BC_Escape, ETriggerEvent::Triggered, this, &ThisClass::OnCmdEscape);

	// ==== Attack Commands ====
	UMWInputUtility::BindInputAction(MWGameplayTags::IMC_BattleCommand_Attack, MWGameplayTags::IATag_BC_Attack_Cancel, ETriggerEvent::Triggered, this, &ThisClass::OnAttackCancel);
	UMWInputUtility::BindInputAction(MWGameplayTags::IMC_BattleCommand_Attack, MWGameplayTags::IATag_BC_Attack_SelectTarget, ETriggerEvent::Triggered, this, &ThisClass::OnSelectAttackTarget);
    UMWInputUtility::BindInputAction(MWGameplayTags::IMC_BattleCommand_Attack, MWGameplayTags::IATag_BC_Attack_C1, ETriggerEvent::Triggered, this, &ThisClass::OnExecuteAttack, FGameplayTag(MWGameplayTags::IATag_BC_Attack_C1));
	UMWInputUtility::BindInputAction(MWGameplayTags::IMC_BattleCommand_Attack, MWGameplayTags::IATag_BC_Attack_C1UD, ETriggerEvent::Triggered, this, &ThisClass::OnExecuteAttack, FGameplayTag(MWGameplayTags::IATag_BC_Attack_C1UD));
	UMWInputUtility::BindInputAction(MWGameplayTags::IMC_BattleCommand_Attack, MWGameplayTags::IATag_BC_Attack_C1LR, ETriggerEvent::Triggered, this, &ThisClass::OnExecuteAttack, FGameplayTag(MWGameplayTags::IATag_BC_Attack_C1LR));
	UMWInputUtility::BindInputAction(MWGameplayTags::IMC_BattleCommand_Attack, MWGameplayTags::IATag_BC_Attack_C2, ETriggerEvent::Triggered, this, &ThisClass::OnExecuteAttack, FGameplayTag(MWGameplayTags::IATag_BC_Attack_C2));
	UMWInputUtility::BindInputAction(MWGameplayTags::IMC_BattleCommand_Attack, MWGameplayTags::IATag_BC_Attack_C2UD, ETriggerEvent::Triggered, this, &ThisClass::OnExecuteAttack, FGameplayTag(MWGameplayTags::IATag_BC_Attack_C2UD));
	UMWInputUtility::BindInputAction(MWGameplayTags::IMC_BattleCommand_Attack, MWGameplayTags::IATag_BC_Attack_C2LR, ETriggerEvent::Triggered, this, &ThisClass::OnExecuteAttack, FGameplayTag(MWGameplayTags::IATag_BC_Attack_C2LR));
    UMWInputUtility::BindInputAction(MWGameplayTags::IMC_BattleCommand_Attack, MWGameplayTags::IATag_BC_Attack_C3, ETriggerEvent::Triggered, this, &ThisClass::OnExecuteAttack, FGameplayTag(MWGameplayTags::IATag_BC_Attack_C3));
	UMWInputUtility::BindInputAction(MWGameplayTags::IMC_BattleCommand_Attack, MWGameplayTags::IATag_BC_Attack_C3UD, ETriggerEvent::Triggered, this, &ThisClass::OnExecuteAttack, FGameplayTag(MWGameplayTags::IATag_BC_Attack_C3UD));
	UMWInputUtility::BindInputAction(MWGameplayTags::IMC_BattleCommand_Attack, MWGameplayTags::IATag_BC_Attack_C3LR, ETriggerEvent::Triggered, this, &ThisClass::OnExecuteAttack, FGameplayTag(MWGameplayTags::IATag_BC_Attack_C3LR));

	// ==== Change Avatar Commands ====
	UMWInputUtility::BindInputAction(MWGameplayTags::IMC_BattleCommand_ChangeAvatar, MWGameplayTags::IATag_BC_ChangeAvatar_SelectTarget, ETriggerEvent::Triggered, this, &ThisClass::OnSelectChangeAvatarTarget);
	UMWInputUtility::BindInputAction(MWGameplayTags::IMC_BattleCommand_ChangeAvatar, MWGameplayTags::IATag_BC_ChangeAvatar_ChangeAvatar, ETriggerEvent::Triggered, this, &ThisClass::OnChangeAvatar);
	UMWInputUtility::BindInputAction(MWGameplayTags::IMC_BattleCommand_ChangeAvatar, MWGameplayTags::IATag_BC_ChangeAvatar_Confirm, ETriggerEvent::Triggered, this, &ThisClass::OnChangeAvatarConfirm);
	UMWInputUtility::BindInputAction(MWGameplayTags::IMC_BattleCommand_ChangeAvatar, MWGameplayTags::IATag_BC_ChangeAvatar_Cancel, ETriggerEvent::Triggered, this, &ThisClass::OnChangeAvatarCancel);

	// ==== Use Item Commands ====
	UMWInputUtility::BindInputAction(MWGameplayTags::IMC_BattleCommand_UseItem, MWGameplayTags::IATag_BC_UseItem_SelectItem, ETriggerEvent::Triggered, this, &ThisClass::OnSelectItem);
	UMWInputUtility::BindInputAction(MWGameplayTags::IMC_BattleCommand_UseItem, MWGameplayTags::IATag_BC_UseItem_SelectTarget, ETriggerEvent::Triggered, this, &ThisClass::OnSelectItemTarget);
	UMWInputUtility::BindInputAction(MWGameplayTags::IMC_BattleCommand_UseItem, MWGameplayTags::IATag_BC_UseItem_Confirm, ETriggerEvent::Triggered, this, &ThisClass::OnUseItemConfirm);
	UMWInputUtility::BindInputAction(MWGameplayTags::IMC_BattleCommand_UseItem, MWGameplayTags::IATag_BC_UseItem_Cancel, ETriggerEvent::Triggered, this, &ThisClass::OnUseItemCancel);

	// ==== Use Spirit Commands ====
	UMWInputUtility::BindInputAction(MWGameplayTags::IMC_BattleCommand_UseSpirit, MWGameplayTags::IATag_BC_UseSpirit_SelectSpirit, ETriggerEvent::Triggered, this, &ThisClass::OnSelectSpirit);
	UMWInputUtility::BindInputAction(MWGameplayTags::IMC_BattleCommand_UseSpirit, MWGameplayTags::IATag_BC_UseSpirit_SelectTarget, ETriggerEvent::Triggered, this, &ThisClass::OnSelectSpiritTarget);
	UMWInputUtility::BindInputAction(MWGameplayTags::IMC_BattleCommand_UseSpirit, MWGameplayTags::IATag_BC_UseSpirit_Confirm, ETriggerEvent::Triggered, this, &ThisClass::OnUseSpiritConfirm);
	UMWInputUtility::BindInputAction(MWGameplayTags::IMC_BattleCommand_UseSpirit, MWGameplayTags::IATag_BC_UseSpirit_Cancel, ETriggerEvent::Triggered, this, &ThisClass::OnUseSpiritCancel);
}

void UMWPlayerActionExecutor::OnCmdAttack(const FInputActionValue& Value)
{
    LogPhaseInfo("OnCmdAttack");

    ChangePhase(EPlayerActionPhase::AttackPreparing);
}

void UMWPlayerActionExecutor::OnCmdChangeAvatar(const FInputActionValue& Value)
{
    LogPhaseInfo("OnCmdChangeAvatar");

    ChangePhase(EPlayerActionPhase::SelectingAvatar);
}

void UMWPlayerActionExecutor::OnCmdUseItem(const FInputActionValue& Value)
{
    LogPhaseInfo("OnCmdUseItem");

    ChangePhase(EPlayerActionPhase::SelectingItem);
}

void UMWPlayerActionExecutor::OnCmdUseSpirit(const FInputActionValue& Value)
{
    LogPhaseInfo("OnCmdUseSpirit");

    ChangePhase(EPlayerActionPhase::SelectingSpirit);
}

void UMWPlayerActionExecutor::OnCmdEscape(const FInputActionValue& Value)
{
    LogPhaseInfo("OnCmdEscape");

    ChangePhase(EPlayerActionPhase::Escaping);
}

void UMWPlayerActionExecutor::OnAttackCancel(const FInputActionValue& Value)
{
    LogPhaseInfo("OnAttackCancel");

    if (TryCancelCurrentAction())
    {
        UMWInputUtility::DisableMappingContext(UGameplayStatics::GetPlayerController(this, 0), MWGameplayTags::IMC_BattleCommand_Attack);
    }
}

void UMWPlayerActionExecutor::OnSelectAttackTarget(const FInputActionValue& Value)
{

    switch (CurrentPhase)
    {
        case EPlayerActionPhase::AttackPreparing:
        LogPhaseInfo("OnSelectAttackTarget");
            // TODO : Update selected target
			break;
        case EPlayerActionPhase::AttackExecuting:
            // Do nothing.
			break;
    }
}

void UMWPlayerActionExecutor::OnExecuteAttack(const FInputActionValue& Value, FGameplayTag Tag)
{
	LogPhaseInfo(FString::Printf(TEXT("OnExecuteAttack : %s"), *Tag.GetTagName().ToString()));

    if (CurrentPhase == EPlayerActionPhase::AttackPreparing)
    {
		ChangePhase(EPlayerActionPhase::AttackExecuting);
    }
}

void UMWPlayerActionExecutor::OnSelectChangeAvatarTarget(const FInputActionValue& Value)
{
	LogPhaseInfo("OnSelectChangeAvatarTarget");
}

void UMWPlayerActionExecutor::OnChangeAvatar(const FInputActionValue& Value)
{
	LogPhaseInfo("OnChangeAvatar");
}

void UMWPlayerActionExecutor::OnChangeAvatarConfirm(const FInputActionValue& Value)
{
    LogPhaseInfo("OnChangeAvatarConfirm");
}

void UMWPlayerActionExecutor::OnChangeAvatarCancel(const FInputActionValue& Value)
{
	LogPhaseInfo("OnChangeAvatarCancel");

    if (TryCancelCurrentAction())
    {
        if (CurrentPhase == EPlayerActionPhase::SelectingAvatar)
        {
		    UMWInputUtility::DisableMappingContext(UGameplayStatics::GetPlayerController(this, 0), MWGameplayTags::IMC_BattleCommand_ChangeAvatar);
        }
    }
}

void UMWPlayerActionExecutor::OnSelectItem(const FInputActionValue& Value)
{
	LogPhaseInfo("OnSelectItem");
}

void UMWPlayerActionExecutor::OnSelectItemTarget(const FInputActionValue& Value)
{
	LogPhaseInfo("OnSelectItemTarget");
}

void UMWPlayerActionExecutor::OnUseItemConfirm(const FInputActionValue& Value)
{
	LogPhaseInfo("OnUseItemConfirm");
}

void UMWPlayerActionExecutor::OnUseItemCancel(const FInputActionValue& Value)
{
	LogPhaseInfo("OnUseItemCancel");

    if (TryCancelCurrentAction())
    {
        if(CurrentPhase == EPlayerActionPhase::SelectingItem)
        {
		    UMWInputUtility::DisableMappingContext(UGameplayStatics::GetPlayerController(this, 0), MWGameplayTags::IMC_BattleCommand_UseItem);
		}
    }
}

void UMWPlayerActionExecutor::OnSelectSpirit(const FInputActionValue& Value)
{
	LogPhaseInfo("OnSelectSpirit");
}

void UMWPlayerActionExecutor::OnSelectSpiritTarget(const FInputActionValue& Value)
{
	LogPhaseInfo("OnSelectSpiritTarget");
}

void UMWPlayerActionExecutor::OnUseSpiritConfirm(const FInputActionValue& Value)
{
	LogPhaseInfo("OnUseSpiritConfirm");
}

void UMWPlayerActionExecutor::OnUseSpiritCancel(const FInputActionValue& Value)
{
	LogPhaseInfo("OnUseSpiritCancel");

    if (TryCancelCurrentAction())
    {
        if (CurrentPhase == EPlayerActionPhase::SelectingSpirit)
        {
            UMWInputUtility::DisableMappingContext(UGameplayStatics::GetPlayerController(this, 0), MWGameplayTags::IMC_BattleCommand_UseSpirit);
		}
    }
}

UE_ENABLE_OPTIMIZATION