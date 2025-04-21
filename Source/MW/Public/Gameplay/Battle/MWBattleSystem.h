// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Gameplay/Battle/MWBattleTypes.h"
#include "Gameplay/MWGameplayTypes.h"
#include "MWBattleSystem.generated.h"

#pragma region Skill
/* Used to trigger gameplay ability for skill. */
DECLARE_MULTICAST_DELEGATE_OneParam(FTriggerSkillAbility, const FGameplayTag&);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnSkillTriggered, int32);
#pragma endregion

#pragma region Target_Select
DECLARE_MULTICAST_DELEGATE_OneParam(FOnTargetSelcted, FMWFoundActorInfo);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnTargetCancelSelcted, FMWFoundActorInfo);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnTargetLocked, FMWFoundActorInfo);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnTargetUnlocked, FMWFoundActorInfo);
#pragma endregion

#pragma region Character
/* Source is the attacker, target is the victim. */
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnStartAttack, AActor*, AActor*);

/* Source is the attacker, target is the victim, location is the hit point. */
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnHit, AActor*, AActor*, const FVector&);

/* Source is the attacker, target is the victim, location is the hit point. */
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnCriticalHit, AActor*, AActor*, const FVector&);

/* Source is the attacker, target is the victim, Location is the attack point. */
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnAttackMiss, AActor*, AActor*, const FVector&);

/* Source is the attacker, target is the victim. */
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHeal, AActor*, AActor*);

#pragma endregion

#pragma region Battle
DECLARE_MULTICAST_DELEGATE(FOnBattleBegin);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnBattleEnd, EBattleResult);

DECLARE_MULTICAST_DELEGATE(FOnBattleBeginPrepareComplete);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnRoundBegin, int32);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnRoundEnd, int32);

/* Begin of the team's turn. */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnTurnBegin, const FMWTeam&);

/* End of the team's turn. */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnTurnEnd, const FMWTeam&);

DECLARE_MULTICAST_DELEGATE(FOnActionComplete);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnInsertMoveAction, int32);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnTeamDied, const FMWTeam&);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnTeamRevive, const FMWTeam&);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnTeamMemberDied, const FMWTeam&);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnTeamMemberRevive, const FMWTeam&);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnActionQueueUpdate, const TArray<FMWTeam>&);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnActionBuffUpdate, const EMWBattleActionBuff&);

#pragma endregion

#pragma region Party
DECLARE_MULTICAST_DELEGATE_OneParam(FOnOneJoinParty, AActor*);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnOneLeaveParty, AActor*);
#pragma endregion

#pragma region Common

#pragma endregion

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class MW_API UMWBattleSystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	FTriggerSkillAbility TriggerSkillAbility;

	FOnSkillTriggered OnSkillTriggered;

	FOnTargetSelcted OnTargetSelected;

	FOnTargetCancelSelcted OnTargetCancelSelected;

	FOnTargetLocked OnTargetLocked;

	FOnTargetUnlocked OnTargetUnlocked;

	FOnBattleBegin OnBattleBegin;

	FOnBattleEnd OnBattleEnd;

	FOnBattleBeginPrepareComplete OnBattleBeginPrepareComplete;

	FOnRoundBegin OnRoundBegin;

	FOnRoundEnd OnRoundEnd;

	FOnTurnBegin OnTurnBegin;

	FOnTurnEnd OnTurnEnd;

	FOnActionComplete OnActionComplete;
	UFUNCTION(BlueprintCallable, Category = "Battle")
	void ActionComplete();

	FOnTeamDied OnTeamDied;

	FOnTeamRevive OnTeamRevive;

	FOnTeamMemberDied OnTeamMemberDied;

	FOnTeamMemberRevive OnTeamMemberRevive;

	FOnActionQueueUpdate OnActionQueueUpdate;

	FOnActionBuffUpdate OnActionBuffUpdate;

	UFUNCTION(BlueprintCallable, Category = "Battle")
	void StartBattle(const TArray<FMWTeam>& InTeams);

	/* Force to end a battle and decide which force is the winner.
	* @param Winner : 0 = player win, 1 = enemy win, 2 = draw
	*/
	UFUNCTION(BlueprintCallable, Category = "Battle")
	void EndBattle(uint8 Winner);

private:
	void OnBattleEndCallback(EBattleResult Result);

	void OnGameModeLogout(AGameModeBase* GameMode, AController* );

private:
	FDelegateHandle DHBattleEnd;

	TObjectPtr<class UMWBattle> BattleInst;
};
