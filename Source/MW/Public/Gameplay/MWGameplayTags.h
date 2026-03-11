// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "NativeGameplayTags.h"

#define DECLARE_MW_GAMEPLAY_TAG(TagName) MW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TagName)

namespace MWGameplayTags
{
	MW_API	FGameplayTag FindTagByString(const FString& TagString, bool bMatchPartialString = false);

	// ==== Basic Input ====
#pragma region Basic Input
	DECLARE_MW_GAMEPLAY_TAG(IMC_Basic);
	DECLARE_MW_GAMEPLAY_TAG(IATag_Basic_Direction);
	DECLARE_MW_GAMEPLAY_TAG(IATag_Basic_Confirm);
	DECLARE_MW_GAMEPLAY_TAG(IATag_Basic_Cancel);
#pragma endregion

	// ==== TPDefault Input ====
#pragma region TPDefault Input
	DECLARE_MW_GAMEPLAY_TAG(IMC_TPDefault);
	DECLARE_MW_GAMEPLAY_TAG(IATag_TPDefault_Debug);
	DECLARE_MW_GAMEPLAY_TAG(IATag_TPDefault_Move);
	DECLARE_MW_GAMEPLAY_TAG(IATag_TPDefault_LookAt);
	DECLARE_MW_GAMEPLAY_TAG(IATag_TPDefault_AutoRun);
#pragma endregion

// ==== Battle Input ====
#pragma region Battle Input
	DECLARE_MW_GAMEPLAY_TAG(IMC_TPBattle);
	// Used for skill of character
	DECLARE_MW_GAMEPLAY_TAG(IATag_TPBattle_CharacterSkillSlot1);
	DECLARE_MW_GAMEPLAY_TAG(IATag_TPBattle_CharacterSkillSlot2);
	DECLARE_MW_GAMEPLAY_TAG(IATag_TPBattle_CharacterSkillSlot3);
	DECLARE_MW_GAMEPLAY_TAG(IATag_TPBattle_CharacterSkillSlot4);
	// Used for skill of partner
	DECLARE_MW_GAMEPLAY_TAG(IATag_TPBattle_PartnerSkillSlot1);
	DECLARE_MW_GAMEPLAY_TAG(IATag_TPBattle_PartnerSkillSlot2);
	DECLARE_MW_GAMEPLAY_TAG(IATag_TPBattle_PartnerSkillSlot3);
	DECLARE_MW_GAMEPLAY_TAG(IATag_TPBattle_PartnerSkillSlot4);
	// Used for extra skill of character
	DECLARE_MW_GAMEPLAY_TAG(IATag_TPBattle_CharacterExtraSkillSlot1);
	DECLARE_MW_GAMEPLAY_TAG(IATag_TPBattle_CharacterExtraSkillSlot2);
	DECLARE_MW_GAMEPLAY_TAG(IATag_TPBattle_CharacterExtraSkillSlot3);
	// Used for charge skill of character
	DECLARE_MW_GAMEPLAY_TAG(IATag_TPBattle_CharacterSkillSlot1_Charge);
	DECLARE_MW_GAMEPLAY_TAG(IATag_TPBattle_CharacterSkillSlot2_Charge);
	DECLARE_MW_GAMEPLAY_TAG(IATag_TPBattle_CharacterSkillSlot3_Charge);
	// Tag for triggered charged skill
	DECLARE_MW_GAMEPLAY_TAG(IATag_TPBattle_ChargeSkillTriggered);

#pragma endregion

	// ==== RPG Battle Command Input ====
#pragma region RPG Battle Command Input

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

	// Input Charge State
	DECLARE_MW_GAMEPLAY_TAG(Ability_InputCharge_Charging);
	DECLARE_MW_GAMEPLAY_TAG(Ability_InputCharge_NoCharge);
	DECLARE_MW_GAMEPLAY_TAG(Ability_InputCharge_Light);
	DECLARE_MW_GAMEPLAY_TAG(Ability_InputCharge_Perfect);
	DECLARE_MW_GAMEPLAY_TAG(Ability_InputCharge_OverCharge);
	DECLARE_MW_GAMEPLAY_TAG(Ability_ActionUninterruptible);


	// Skill
	DECLARE_MW_GAMEPLAY_TAG(Ability_Skill_Chainable);


	// ==== RPG Battle Command ====
#pragma region RPG Battle Command
	DECLARE_MW_GAMEPLAY_TAG(Battle_Ability_UnitAttack);
	// Attack command
	DECLARE_MW_GAMEPLAY_TAG(Battle_ComboAttack_P1);
	DECLARE_MW_GAMEPLAY_TAG(Battle_ComboAttack_P2);
	DECLARE_MW_GAMEPLAY_TAG(Battle_ComboAttack_P3);
	// Combo
	DECLARE_MW_GAMEPLAY_TAG(Battle_ComboCentral);
	DECLARE_MW_GAMEPLAY_TAG(Battle_ComboUpDown);
	DECLARE_MW_GAMEPLAY_TAG(Battle_ComboLeftRight);
	// Combo Hit
	DECLARE_MW_GAMEPLAY_TAG(Battle_ComboHit);
	DECLARE_MW_GAMEPLAY_TAG(Battle_AvatarChanged);

	// Character
	DECLARE_MW_GAMEPLAY_TAG(Character_CombatState);
#pragma endregion


#pragma endregion

	// ==== SetByCaller Tag ====
#pragma region SetByCaller Tag
	DECLARE_MW_GAMEPLAY_TAG(GP_SetByCaller_ComboHitRatio);
#pragma endregion
};
