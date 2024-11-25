// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Battle/MWBattleDefines.h"
#include "Gameplay/MWGameplayDefines.h"
#include "MWBattleSystem.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class MW_API UMWBattleSystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Battle")
	FTryTriggerWeak TryTriggerWeak;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Battle")
	FOnWeakTriggered OnWeakTriggered;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Battle")
	FTriggerSkillAbility TriggerSkillAbility;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Battle")
	FOnSkillTriggered OnSkillTriggered;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Battle")
	FOnTargetSelcted OnTargetSelected;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Battle")
	FOnTargetCancelSelcted OnTargetCancelSelected;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Battle")
	FOnCharacterDied OnCharacterDied;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Battle")
	FOnAttack OnAttack;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Battle")
	FOnTargetLocked OnTargetLocked;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Battle")
	FOnTargetUnlocked OnTargetUnlocked;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Party")
	FOnPartyUpdate OnPartyUpdate;
};
