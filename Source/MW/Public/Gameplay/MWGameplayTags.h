// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "NativeGameplayTags.h"

namespace MWGameplayTags
{
	MW_API	FGameplayTag FindTagByString(const FString& TagString, bool bMatchPartialString = false);

	// Declare all of the custom native tags that MW will use
	//MW_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_IsDead);
	//MW_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_Cooldown);
	//MW_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_Cost);
	//MW_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_TagsBlocked);
	//MW_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_TagsMissing);
	//MW_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_Networking);
	//MW_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_ActivationGroup);

	MW_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Behavior_SurvivesDeath);

	MW_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Debug);
	MW_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Move);
	MW_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_LookAt);
	//MW_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look_Stick);
	//MW_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Crouch);
	MW_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_AutoRun);

	//MW_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_Spawned);
	//MW_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_DataAvailable);
	//MW_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_DataInitialized);
	//MW_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_GameplayReady);

	//MW_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_Death);
	//MW_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_Reset);
	//MW_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_RequestReset);

	//MW_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(SetByCaller_Damage);
	//MW_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(SetByCaller_Heal);

	//MW_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cheat_GodMode);
	//MW_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cheat_UnlimitedHealth);

	//MW_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Crouching);
	//MW_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_AutoRunning);
	//MW_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Death);
	//MW_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Death_Dying);
	//MW_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Death_Dead);

	//// These are mappings from MovementMode enums to GameplayTags associated with those enums (below)
	//MW_API	extern const TMap<uint8, FGameplayTag> MovementModeTagMap;
	//MW_API	extern const TMap<uint8, FGameplayTag> CustomMovementModeTagMap;

	//MW_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Walking);
	//MW_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_NavWalking);
	//MW_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Falling);
	//MW_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Swimming);
	//MW_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Flying);

	//MW_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Custom);
};
