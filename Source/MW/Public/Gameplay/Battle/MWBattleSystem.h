// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Gameplay/Battle/MWBattleTypes.h"
#include "Gameplay/MWGameplayTypes.h"
#include "MWBattleSystem.generated.h"

#pragma region Skill
/* Used to trigger gameplay ability for skill. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTriggerSkillAbility, const FGameplayTag&, Tag);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSkillTriggered, int32, SkillId);
#pragma endregion

#pragma region Target_Select
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTargetSelcted, FMWFoundActorInfo, TargetInfo);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTargetCancelSelcted, FMWFoundActorInfo, TargetInfo);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTargetLocked, FMWFoundActorInfo, Target);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTargetUnlocked, FMWFoundActorInfo, Target);
#pragma endregion

#pragma region Character
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDied, AActor*, Actor);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRevive, AActor*, Actor);

/* Source is the attacker, target is the victim. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStartAttack, AActor*, Source, AActor*, Actor);

/* Source is the attacker, target is the victim, location is the hit point. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHit, AActor*, Source, AActor*, Target, const FVector&, Location);

/* Source is the attacker, target is the victim, location is the hit point. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCriticalHit, AActor*, Source, AActor*, Target, const FVector&, Location);

/* Source is the attacker, target is the victim, Location is the attack point. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAttackMiss, AActor*, Source, AActor*, Target, const FVector&, Location);

/* Source is the attacker, target is the victim. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHeal, AActor*, Source, AActor*, Target);

#pragma endregion

#pragma region Party
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOneJoinParty, AActor*, Actor);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOneLeaveParty, AActor*, Actor);
#pragma endregion

#pragma region Common
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBattleStart);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBattleEnd, bool, IsVectory);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRoundBegin);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRoundEnd);

/* begin of the actor's turn. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTurnBegin, AActor*, Actor);

/* end of the actor's turn. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTurnEnd, AActor*, Actor);

#pragma endregion

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class MW_API UMWBattleSystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Battle")
	FTriggerSkillAbility TriggerSkillAbility;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Battle")
	FOnSkillTriggered OnSkillTriggered;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Battle")
	FOnTargetSelcted OnTargetSelected;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Battle")
	FOnTargetCancelSelcted OnTargetCancelSelected;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Battle")
	FOnDied OnCharacterDied;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Battle")
	FOnTargetLocked OnTargetLocked;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Battle")
	FOnTargetUnlocked OnTargetUnlocked;
};
