// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Delegates/Delegate.h"
#include "Gameplay/MWGameplayTypes.h"
#include "MWBattleTypes.generated.h"

//------------------ Macro ------------------//
#pragma region Skill
#define MW_SKILL_NUM 3
#define MW_SKILL_GROUP_NUM 3
#pragma endregion

#pragma region Weak System
#define MW_WEAK_EXPOSURE_MIN 0
#define MW_WEAK_EXPOSURE_MAX 100
#pragma endregion



USTRUCT()
struct FPlaceHolder
{
	GENERATED_BODY()
};

#pragma region Weak System
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FTryTriggerWeak, UObject*, Owner, const FGameplayTag&, WeakTag, int32, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeakTriggered, UObject*, Owner, int32, Times);
#pragma endregion

#pragma region Skill
/* Used to trigger gameplay ability for skill. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTriggerSkillAbility, const FGameplayTag, Tag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSkillTriggered, int32, SkillId);
#pragma endregion

#pragma region Target Select
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTargetSelcted, FMWFoundActorInfo, TargetInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTargetCancelSelcted, FMWFoundActorInfo, TargetInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAttack, FMWFoundActorInfo, Attacker, FMWFoundActorInfo, Attackee);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTargetLocked, FMWFoundActorInfo, Target);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTargetUnlocked, FMWFoundActorInfo, Target);
#pragma endregion

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterDied, FMWFoundActorInfo, TargetInfo);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPartyUpdate, FPartyInfo, Info);