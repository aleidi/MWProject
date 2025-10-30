// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "NativeGameplayTags.h"

#define DECLARE_MW_GAMEPLAY_TAG(TagName) MW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TagName)

namespace MWGameplayTags
{
	MW_API	FGameplayTag FindTagByString(const FString& TagString, bool bMatchPartialString = false);

//**********************//
//      Action Tag      //
//**********************//
#pragma region Input Action Tag

//**********************//
//        Basic         //
//**********************//
#pragma region Basic
	DECLARE_MW_GAMEPLAY_TAG(IATag_Basic_Direction);
	DECLARE_MW_GAMEPLAY_TAG(IATag_Basic_Confirm);
	DECLARE_MW_GAMEPLAY_TAG(IATag_Basic_Cancel);
#pragma endregion

//**********************//
//   TPDefault Input    //
//**********************//
#pragma region TPDefault
	DECLARE_MW_GAMEPLAY_TAG(IATag_TPDefault_Debug);
	DECLARE_MW_GAMEPLAY_TAG(IATag_TPDefault_Move);
	DECLARE_MW_GAMEPLAY_TAG(IATag_TPDefault_LookAt);
	DECLARE_MW_GAMEPLAY_TAG(IATag_TPDefault_AutoRun);
#pragma endregion

//**********************//
//    Battle Command    //
//**********************//
#pragma region Battle Command
	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_Move);
	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_ChangeLeader);
	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_UseItem);
	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_UseSpirit);
	/* Battle Command tags container. */
	extern TArray<FGameplayTag> BattleCommands;

	// Move Command
	/* Character 1 control. */
	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_Move_C1);
	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_Move_C1U);
	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_Move_C1D);
	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_Move_C1L);
	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_Move_C1R);
	/* Character 2 control. */
	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_Move_C2);
	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_Move_C2U);
	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_Move_C2D);
	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_Move_C2L);
	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_Move_C2R);
	/* Character 3 control. */
	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_Move_C3);
	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_Move_C3U);
	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_Move_C3D);
	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_Move_C3L);
	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_Move_C3R);
	/* Character action tags container. */
	extern TArray<FGameplayTag> BattleCharacterAction;

	//DECLARE_MW_GAMEPLAY_TAG(IATag_CC_SupportAttack1);
	//DECLARE_MW_GAMEPLAY_TAG(IATag_CC_SupportAttack2);
	//DECLARE_MW_GAMEPLAY_TAG(IATag_CC_UltimateSkill);
#pragma endregion

#pragma endregion

//**********************//
//    Input Mapping     //
//**********************//
#pragma region Input Mapping Tag
	DECLARE_MW_GAMEPLAY_TAG(IMC_Basic);
	DECLARE_MW_GAMEPLAY_TAG(IMC_TPDefault);
	DECLARE_MW_GAMEPLAY_TAG(IMC_BattleCommand);
	DECLARE_MW_GAMEPLAY_TAG(IMC_BattleCharacterAction);
#pragma endregion

//**********************//
//     Gameplay Tag     //
//**********************//
#pragma region Gameplay Tag
	DECLARE_MW_GAMEPLAY_TAG(GP_Battle_Ability_UnitAttack);
	DECLARE_MW_GAMEPLAY_TAG(GP_Battle_ComboAttack);
	DECLARE_MW_GAMEPLAY_TAG(GP_Battle_ComboCentral);
	DECLARE_MW_GAMEPLAY_TAG(GP_Battle_ComboUpDown);
	DECLARE_MW_GAMEPLAY_TAG(GP_Battle_ComboLeftRight);
	DECLARE_MW_GAMEPLAY_TAG(GP_Battle_Event_AvatarChanged);
#pragma endregion
};
