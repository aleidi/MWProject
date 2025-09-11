// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "NativeGameplayTags.h"

#define DECLARE_MW_GAMEPLAY_TAG(TagName) MW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TagName)

namespace MWGameplayTags
{
	MW_API	FGameplayTag FindTagByString(const FString& TagString, bool bMatchPartialString = false);

#pragma region ActionTag

#pragma region Basic
	// Basic
	DECLARE_MW_GAMEPLAY_TAG(IATag_Basic_DirectionalInput);
	DECLARE_MW_GAMEPLAY_TAG(IATag_Basic_Confirm);
	DECLARE_MW_GAMEPLAY_TAG(IATag_Basic_Cancel);
#pragma endregion


	// Default Input
	DECLARE_MW_GAMEPLAY_TAG(IATag_Default_Debug);
	DECLARE_MW_GAMEPLAY_TAG(IATag_Default_Move);
	DECLARE_MW_GAMEPLAY_TAG(IATag_Default_LookAt);
	DECLARE_MW_GAMEPLAY_TAG(IATag_Default_AutoRun);

	// Battle Command
	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_Move);
	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_ChangeLeader);
	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_UseItem);
	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_UseSpirit);

	// Combat Command
	DECLARE_MW_GAMEPLAY_TAG(IATag_CC_Attack_Up);
	DECLARE_MW_GAMEPLAY_TAG(IATag_CC_Attack_Down);
	DECLARE_MW_GAMEPLAY_TAG(IATag_CC_Attack_Left);
	DECLARE_MW_GAMEPLAY_TAG(IATag_CC_Attack_Right);
	DECLARE_MW_GAMEPLAY_TAG(IATag_CC_Dir_Up);
	DECLARE_MW_GAMEPLAY_TAG(IATag_CC_Dir_Down);
	DECLARE_MW_GAMEPLAY_TAG(IATag_CC_Dir_Left);
	DECLARE_MW_GAMEPLAY_TAG(IATag_CC_Dir_Right);
	DECLARE_MW_GAMEPLAY_TAG(IATag_CC_SupportAttack1);
	DECLARE_MW_GAMEPLAY_TAG(IATag_CC_SupportAttack2);
	DECLARE_MW_GAMEPLAY_TAG(IATag_CC_UltimateSkill);
#pragma endregion

#pragma region InputMappingTag
	DECLARE_MW_GAMEPLAY_TAG(IMCTag_Basic);
	DECLARE_MW_GAMEPLAY_TAG(IMCTag_BattleCommand);
	DECLARE_MW_GAMEPLAY_TAG(IMCTag_CombatCommand);
#pragma endregion
};
