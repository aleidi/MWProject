#pragma once

// Include
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InputActionValue.h"
#include "Define/MWDefineGameplay.h"
#include "Gameplay/MWGameplayTags.h"
#include "MWActionExecutor.generated.h"

// Forward Declare

// Define

/**
 * Action Phase
 */
UENUM(BlueprintType)
enum class EPlayerActionPhase : uint8
{
    Idle,
    
    // ===== Command Choice Phase =====
    WaitingCommand,			// Show Menu：Move/ChangeAvatar/UseItem/UseSpirit
    
    // ===== Attack Phase =====
	AttackPreparing,		// Selecting attack target, can be canceled back to preparation
	AttackExecuting,		// Executing attack, cannot be canceled

	// ===== ChangeAvatar Phase =====
	SelectingAvatar,		// Display avatar selection interface, can be canceled
	ChangingAvatar,			// Execute avatar change animation/logic

	// ===== UseItem Phase =====
	SelectingItem,			// Display item list, can be canceled
	SelectingItemTarget,	// Selecting target for item usage (if needed), can be canceled
	UsingItem,				// Executing item usage

	// ===== UseSpirit Phase =====
	SelectingSpirit,		// Display spirit skill list, can be canceled
	SelectingSpiritTarget,	// Selecting skill target (if needed), can be canceled
	UsingSpirit,			// Executing spirit skill

	// ==== Escape ====
	Escaping,				// Executing escape logic

	// ===== General Phase =====
	Returning,				// Returning to start point
	TurnComplete,			// Turn completed
    
    Max						UMETA(Hidden)
};

/**
 * ActionType
 */
UENUM(BlueprintType)
enum class EPlayerActionType : uint8
{
    None,
    Attack,
    ChangeAvatar,
    UseItem,
    UseSpirit,
    
    Max						UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FMWPlayerActionContext
{
	GENERATED_BODY()

	// ==== Character Info ====
	TArray<FVector> StartPositions;
	TArray<FRotator> StartRotations;

	// ==== Action Type ====
	EPlayerActionType CurrentActionType = EPlayerActionType::None;
	EPlayerActionType LastActionType = EPlayerActionType::None;

	// ==== Action Points====
	int32 ActionPoints = 100;

	// ==== Select ====
	TObjectPtr<AActor> AttackTarget = nullptr;

	// ==== Time ====
	float TotalTime = 0.f;
	float LastInputTime = 0.f;
	float InputTimeoutDuration = 3.f;
	float EnterPhaseTime = 0.f;

	// ==== State ====
	bool bCanCancelCurrentAction = false;
	bool bActionDone = false;

	bool IsInputTimeout() const;

	void RecordInput();

	void RecordEnterPhaseTime();

	float GetTimeSinceEnterPhase() const;

	bool CanCancelAction() const;

	void Reset();
};

struct FMWActionExecutorData
{
};

/*
 * @class FMWActionExecutor
 * 
 * @brief
 *
 * @note
 */
 UCLASS()
class UMWActionExecutor : public UObject
{
	 GENERATED_BODY()

public:
	UMWActionExecutor() = default;

	virtual void Init() {}
	virtual void Uninit() {}

	/*
	 * Called every frame and check if the turn action is finished by info passed in.
	 *
	 * @param InInfo : the info that used to check if the action is finished.
	 * @param OutIsComplete : true means the action is finished, false means it's still going on.
	 */
	virtual void Update(float DeltaTime, const FMWActionExecutorData& InInfo, bool& OutIsComplete) {}

	void SetActionUnits(const FMWTeam& InPlayerTeam, const FMWTeam& InEnemyTeam);

protected:
	virtual AActor* GetTarget() { return nullptr; }

protected:
	UPROPERTY()
	int32 ActionsPoints = 0;

	float time = 0.f;

	bool bActionCompleted = false;

	FMWTeam PlayerTeam;

	FMWTeam EnemyTeam;
};

UCLASS()
class UMWEnemyActionExecutor : public UMWActionExecutor
{
	GENERATED_BODY()

public:
	virtual void Init() override;

	virtual void Update(float DeltaTime, const FMWActionExecutorData& InInfo, bool& OutIsComplete);
};

UCLASS()
class UMWPlayerActionExecutor : public UMWActionExecutor
{
	GENERATED_BODY()

private:
	

public:
	virtual void Init() override;

	virtual void Uninit() override;

	virtual void Update(float DeltaTime, const FMWActionExecutorData& InInfo, bool& OutIsComplete);

	// ==== Action Phase ====
public:
	// Try to cancel current action phase, return to last cancellable phase.
	// 現在のアクションフェーズをキャンセルして、最後のキャンセル可能なフェーズに戻る.
	bool TryCancelCurrentAction();

	// Check if can cancel current action phase.
	// 現在のアクションフェーズをキャンセルできるか確認する.
	bool CanCancelAction() const;

private:
	void UpdateActionPhase(float DeltaTime);

	void ChangePhase(EPlayerActionPhase NewPhase);

	void OnEnterPhase(EPlayerActionPhase Phase);

	void OnExitPhase(EPlayerActionPhase Phase);

	void InitActionPhase();

	void ReturnToCommandSelection();

	// Phase Enter
	void OnEnterPhase_Idle();
	void OnEnterPhase_WaitingCommand();
	void OnEnterPhase_AttackPreparing();
	void OnEnterPhase_AttackExecuting();
	void OnEnterPhase_SelectingAvatar();
	void OnEnterPhase_ChangingAvatar();
	void OnEnterPhase_SelectingItem();
	void OnEnterPhase_SelectingItemTarget();
	void OnEnterPhase_UsingItem();
	void OnEnterPhase_SelectingSpirit();
	void OnEnterPhase_SelectingSpiritTarget();
	void OnEnterPhase_UsingSpirit();
	void OnEnterPhase_Escaping();
	void OnEnterPhase_Returning();
	void OnEnterPhase_TurnComplete();

	// Phase Exit
	void OnExitPhase_Idle();
	void OnExitPhase_WaitingCommand();
	void OnExitPhase_AttackPreparing();
	void OnExitPhase_AttackExecuting();
	void OnExitPhase_SelectingAvatar();
	void OnExitPhase_ChangingAvatar();
	void OnExitPhase_SelectingItem();
	void OnExitPhase_SelectingItemTarget();
	void OnExitPhase_UsingItem();
	void OnExitPhase_SelectingSpirit();
	void OnExitPhase_SelectingSpiritTarget();
	void OnExitPhase_UsingSpirit();
	void OnExitPhase_Escaping();
	void OnExitPhase_Returning();
	void OnExitPhase_TurnComplete();

	// Phase Update
	void UpdatePhase_Idle(float DeltaTime);
	void UpdatePhase_WaitingCommand(float DeltaTime);
	void UpdatePhase_AttackPreparing(float DeltaTime);
	void UpdatePhase_AttackExecuting(float DeltaTime);
	void UpdatePhase_SelectingAvatar(float DeltaTime);
	void UpdatePhase_ChangingAvatar(float DeltaTime);
	void UpdatePhase_SelectingItem(float DeltaTime);
	void UpdatePhase_SelectingItemTarget(float DeltaTime);
	void UpdatePhase_UsingItem(float DeltaTime);
	void UpdatePhase_SelectingSpirit(float DeltaTime);
	void UpdatePhase_SelectingSpiritTarget(float DeltaTime);
	void UpdatePhase_UsingSpirit(float DeltaTime);
	void UpdatePhase_Escaping(float DeltaTime);
	void UpdatePhase_Returning(float DeltaTime);
	void UpdatePhase_TurnComplete(float DeltaTime);

	void LogPhaseInfo(const FString& Message, bool bIsWarning = false) const;

private:
	/* Display action related ui.*/
	void DisplayUI();

	/* Mapping input for TurnAction.*/
	void SetupInput();


	// ==== Input callback ====

	// == WaitCommand ==
	void OnCmdAttack(const FInputActionValue& Value);
	void OnCmdChangeAvatar(const FInputActionValue& Value);
	void OnCmdUseItem(const FInputActionValue& Value);
	void OnCmdUseSpirit(const FInputActionValue& Value);
	void OnCmdEscape(const FInputActionValue& Value);

	// == Attack ==
	void OnAttackCancel(const FInputActionValue& Value);
	void OnSelectAttackTarget(const FInputActionValue& Value);
	void OnExecuteAttack(const FInputActionValue& Value, FGameplayTag Tag);

	// == Change Avatar ==
	void OnSelectChangeAvatarTarget(const FInputActionValue& Value);
	void OnChangeAvatar(const FInputActionValue& Value);
	void OnChangeAvatarConfirm(const FInputActionValue& Value);
	void OnChangeAvatarCancel(const FInputActionValue& Value);

	// == Use Item ==
	void OnSelectItem(const FInputActionValue& Value);
	void OnSelectItemTarget(const FInputActionValue& Value);
	void OnUseItemConfirm(const FInputActionValue& Value);
	void OnUseItemCancel(const FInputActionValue& Value);

	// == Use Spirit ==
	void OnSelectSpirit(const FInputActionValue& Value);
	void OnSelectSpiritTarget(const FInputActionValue& Value);
	void OnUseSpiritConfirm(const FInputActionValue& Value);
	void OnUseSpiritCancel(const FInputActionValue& Value);

private:
	EPlayerActionPhase CurrentPhase = EPlayerActionPhase::Idle;
	FMWPlayerActionContext ActionContext;
};