#pragma once

// Include
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InputActionValue.h"
#include "Define/MWDefineGameplay.h"
#include "MWTurnAction.generated.h"

// Forward Declare

// Macro

struct FMWTurnActionData
{
};

/*
 * @class FMWTurnAction
 * 
 * @brief
 *
 * @note
 */
 UCLASS()
class UMWTurnAction : public UObject
{
	 GENERATED_BODY()

public:
	UMWTurnAction() = default;

	virtual void Init() {}
	virtual void Uninit() {}

	/*
	 * Called every frame and check if the turn action is finished by info passed in.
	 *
	 * @param InInfo : the info that used to check if the action is finished.
	 * @param OutIsFin : true means the action is finished, false means it's still going on.
	 */
	virtual void Update(const FMWTurnActionData& InInfo, bool& OutIsFin) {}

	void SetActionUnits(const TArray<FMWTeam>& InUnits);

protected:
	UPROPERTY()
	int32 ActionsPoints = 0;

	float time = 0.f;

	bool bActionFinished = false;

	TArray<FMWTeam> ActionUnits;
};

UCLASS()
class UMWEnemyTurnAction : public UMWTurnAction
{
	GENERATED_BODY()

public:
	virtual void Init() override;

	virtual void Update(const FMWTurnActionData& InInfo, bool& OutIsFin);
};

UCLASS()
class UMWPlayerTurnAction : public UMWTurnAction
{
	GENERATED_BODY()

public:
	virtual void Init() override;

	virtual void Uninit() override;

	virtual void Update(const FMWTurnActionData& InInfo, bool& OutIsFin);
protected:
	/* Display action related ui.*/
	virtual void DisplayUI();

	/* Mapping input for TurnAction.*/
	virtual void SetupInput();

	void OnMove(const FInputActionValue& Value);
	void OnChangeLeader(const FInputActionValue& Value);
	void OnUseItem(const FInputActionValue& Value);
	void OnUseSpirit(const FInputActionValue& Value);
};