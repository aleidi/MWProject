// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "NativeGameplayTags.h"

#define DECLARE_MW_GAMEPLAY_TAG(TagName) MW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TagName)

namespace MWGameplayTags
{
	MW_API	FGameplayTag FindTagByString(const FString& TagString, bool bMatchPartialString = false);

	// ==== Basic ====
#pragma region Basic
	DECLARE_MW_GAMEPLAY_TAG(IMC_Basic);
	DECLARE_MW_GAMEPLAY_TAG(IATag_Basic_Direction);
	DECLARE_MW_GAMEPLAY_TAG(IATag_Basic_Confirm);
	DECLARE_MW_GAMEPLAY_TAG(IATag_Basic_Cancel);
#pragma endregion

	// ==== TPDefault Input ====
#pragma region TPDefault
	DECLARE_MW_GAMEPLAY_TAG(IMC_TPDefault);
	DECLARE_MW_GAMEPLAY_TAG(IATag_TPDefault_Debug);
	DECLARE_MW_GAMEPLAY_TAG(IATag_TPDefault_Move);
	DECLARE_MW_GAMEPLAY_TAG(IATag_TPDefault_LookAt);
	DECLARE_MW_GAMEPLAY_TAG(IATag_TPDefault_AutoRun);
#pragma endregion

	// ==== Battle Command ====
#pragma region Battle Command

	// == General ==
	DECLARE_MW_GAMEPLAY_TAG(IMC_BattleCommand);

	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_Attack);
	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_ChangeAvatar);
	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_UseItem);
	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_UseSpirit);
	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_Escape);

	// == Attack ==
	DECLARE_MW_GAMEPLAY_TAG(IMC_BattleCommand_Attack);

	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_Attack_SelectTarget);
	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_Attack_Cancel);

	// Character 1 control.
	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_Attack_C1);
	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_Attack_C1UD);
	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_Attack_C1LR);
	// Character 2 control.
	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_Attack_C2);
	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_Attack_C2UD);
	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_Attack_C2LR);
	// Character 3 control.
	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_Attack_C3);
	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_Attack_C3UD);
	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_Attack_C3LR);

	// Character action tags container.
	extern TArray<FGameplayTag> BattleCharacterAttack;

	// == ChangeAvatar ==
	DECLARE_MW_GAMEPLAY_TAG(IMC_BattleCommand_ChangeAvatar);

	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_ChangeAvatar_Confirm);
	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_ChangeAvatar_Cancel);

	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_ChangeAvatar_SelectTarget);
	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_ChangeAvatar_ChangeAvatar);

	// == UseItem ==
	DECLARE_MW_GAMEPLAY_TAG(IMC_BattleCommand_UseItem);

	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_UseItem_Confirm);
	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_UseItem_Cancel);

	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_UseItem_SelectItem);
	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_UseItem_SelectTarget);

	// == UseSprit ==
	DECLARE_MW_GAMEPLAY_TAG(IMC_BattleCommand_UseSpirit);

	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_UseSpirit_Confirm);
	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_UseSpirit_Cancel);

	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_UseSpirit_SelectSpirit);
	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_UseSpirit_SelectTarget);

	// Battle Command tags container.
	extern TArray<FGameplayTag> BattleCommands;
#pragma endregion

	// ==== Gameplay Tag ====
#pragma region Gameplay Tag
	// Gameplay Ability
	DECLARE_MW_GAMEPLAY_TAG(Ability_ActivateFail_ActivationGroup);
	DECLARE_MW_GAMEPLAY_TAG(Ability_Input_Charging);

	DECLARE_MW_GAMEPLAY_TAG(GP_Battle_Ability_UnitAttack);
	// Attack command
	DECLARE_MW_GAMEPLAY_TAG(GP_Battle_ComboAttack_P1);
	DECLARE_MW_GAMEPLAY_TAG(GP_Battle_ComboAttack_P2);
	DECLARE_MW_GAMEPLAY_TAG(GP_Battle_ComboAttack_P3);
	// Combo
	DECLARE_MW_GAMEPLAY_TAG(GP_Battle_ComboCentral);
	DECLARE_MW_GAMEPLAY_TAG(GP_Battle_ComboUpDown);
	DECLARE_MW_GAMEPLAY_TAG(GP_Battle_ComboLeftRight);
	// Combo Hit
	DECLARE_MW_GAMEPLAY_TAG(GP_Battle_ComboHit);
	DECLARE_MW_GAMEPLAY_TAG(GP_Battle_AvatarChanged);

	// Character
	DECLARE_MW_GAMEPLAY_TAG(GP_Character_CombatState);
#pragma endregion

	// ==== SetByCaller Tag ====
#pragma region SetByCaller Tag
	DECLARE_MW_GAMEPLAY_TAG(GP_SetByCaller_ComboHitRatio);
#pragma endregion
};
