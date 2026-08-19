// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "NativeGameplayTags.h"

#define DECLARE_MW_GAMEPLAY_TAG(TagName) MW_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TagName)

namespace MWGameplayTags
{
	MW_API	FGameplayTag FindTagByString(const FString& TagString, bool bMatchPartialString = false);

	// ==== 基本入力 ====
#pragma region Basic Input
	DECLARE_MW_GAMEPLAY_TAG(IMC_Basic);
	DECLARE_MW_GAMEPLAY_TAG(IATag_Basic_Direction);
	DECLARE_MW_GAMEPLAY_TAG(IATag_Basic_Confirm);
	DECLARE_MW_GAMEPLAY_TAG(IATag_Basic_Cancel);
#pragma endregion

	// ==== TPDefault入力 ====
#pragma region TPDefault Input
	DECLARE_MW_GAMEPLAY_TAG(IMC_TPDefault);
	DECLARE_MW_GAMEPLAY_TAG(IATag_TPDefault_Debug);
	DECLARE_MW_GAMEPLAY_TAG(IATag_TPDefault_Move);
	DECLARE_MW_GAMEPLAY_TAG(IATag_TPDefault_LookAt);
	DECLARE_MW_GAMEPLAY_TAG(IATag_TPDefault_AutoRun);
#pragma endregion

// ==== バトル入力 ====
#pragma region Battle Input
	DECLARE_MW_GAMEPLAY_TAG(IMC_TPBattle);
	// キャラクタースキル用
	DECLARE_MW_GAMEPLAY_TAG(IATag_TPBattle_CharacterSkillSlot1);
	DECLARE_MW_GAMEPLAY_TAG(IATag_TPBattle_CharacterSkillSlot2);
	DECLARE_MW_GAMEPLAY_TAG(IATag_TPBattle_CharacterSkillSlot3);
	DECLARE_MW_GAMEPLAY_TAG(IATag_TPBattle_CharacterSkillSlot4);
	// パートナースキル用
	DECLARE_MW_GAMEPLAY_TAG(IATag_TPBattle_PartnerSkillSlot1);
	DECLARE_MW_GAMEPLAY_TAG(IATag_TPBattle_PartnerSkillSlot2);
	DECLARE_MW_GAMEPLAY_TAG(IATag_TPBattle_PartnerSkillSlot3);
	DECLARE_MW_GAMEPLAY_TAG(IATag_TPBattle_PartnerSkillSlot4);
	// キャラクターの追加スキル用
	DECLARE_MW_GAMEPLAY_TAG(IATag_TPBattle_CharacterExtraSkillSlot1);
	DECLARE_MW_GAMEPLAY_TAG(IATag_TPBattle_CharacterExtraSkillSlot2);
	DECLARE_MW_GAMEPLAY_TAG(IATag_TPBattle_CharacterExtraSkillSlot3);
	// キャラクターのチャージスキル用
	DECLARE_MW_GAMEPLAY_TAG(IATag_TPBattle_CharacterSkillSlot1_Charge);
	DECLARE_MW_GAMEPLAY_TAG(IATag_TPBattle_CharacterSkillSlot2_Charge);
	DECLARE_MW_GAMEPLAY_TAG(IATag_TPBattle_CharacterSkillSlot3_Charge);
	// チャージ済みスキル用Tag
	DECLARE_MW_GAMEPLAY_TAG(IATag_TPBattle_ChargeSkill);
#pragma endregion

	// ==== RPGバトルコマンド入力 ====
#pragma region RPG Battle Command Input

	// == 共通 ==
	DECLARE_MW_GAMEPLAY_TAG(IMC_BattleCommand);

	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_Attack);
	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_ChangeAvatar);
	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_UseItem);
	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_UseSpirit);
	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_Escape);

	// == 攻撃 ==
	DECLARE_MW_GAMEPLAY_TAG(IMC_BattleCommand_Attack);

	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_Attack_SelectTarget);
	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_Attack_Cancel);

	// キャラクター1操作。
	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_Attack_C1);
	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_Attack_C1UD);
	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_Attack_C1LR);
	// キャラクター2操作。
	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_Attack_C2);
	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_Attack_C2UD);
	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_Attack_C2LR);
	// キャラクター3操作。
	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_Attack_C3);
	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_Attack_C3UD);
	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_Attack_C3LR);

	// キャラクターActionTagコンテナ。
	extern TArray<FGameplayTag> BattleCharacterAttack;

	// == Avatar切替 ==
	DECLARE_MW_GAMEPLAY_TAG(IMC_BattleCommand_ChangeAvatar);

	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_ChangeAvatar_Confirm);
	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_ChangeAvatar_Cancel);

	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_ChangeAvatar_SelectTarget);
	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_ChangeAvatar_ChangeAvatar);

	// == Item使用 ==
	DECLARE_MW_GAMEPLAY_TAG(IMC_BattleCommand_UseItem);

	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_UseItem_Confirm);
	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_UseItem_Cancel);

	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_UseItem_SelectItem);
	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_UseItem_SelectTarget);

	// == Spirit使用 ==
	DECLARE_MW_GAMEPLAY_TAG(IMC_BattleCommand_UseSpirit);

	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_UseSpirit_Confirm);
	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_UseSpirit_Cancel);

	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_UseSpirit_SelectSpirit);
	DECLARE_MW_GAMEPLAY_TAG(IATag_BC_UseSpirit_SelectTarget);

	// バトルCommandTagコンテナ。
	extern TArray<FGameplayTag> BattleCommands;
#pragma endregion

	// ==== GameplayTag ====
#pragma region Gameplay Tag
	// GameplayAbility
	DECLARE_MW_GAMEPLAY_TAG(Ability_ActivateFail_ActivationGroup);

	// 入力チャージ状態
	DECLARE_MW_GAMEPLAY_TAG(Ability_InputCharge_Charging);
	DECLARE_MW_GAMEPLAY_TAG(Ability_InputCharge_NoCharge);
	DECLARE_MW_GAMEPLAY_TAG(Ability_InputCharge_Light);
	DECLARE_MW_GAMEPLAY_TAG(Ability_InputCharge_Perfect);
	DECLARE_MW_GAMEPLAY_TAG(Ability_InputCharge_OverCharge);
	DECLARE_MW_GAMEPLAY_TAG(Ability_ActionUninterruptible);

	// スキル
	DECLARE_MW_GAMEPLAY_TAG(Ability_Skill_Chainable);
	DECLARE_MW_GAMEPLAY_TAG(Ability_Skill_Cast);

	// ==== RPGバトルコマンド ====
#pragma region RPG Battle Command
	DECLARE_MW_GAMEPLAY_TAG(Battle_Ability_UnitAttack);
	// 攻撃コマンド
	DECLARE_MW_GAMEPLAY_TAG(Battle_ComboAttack_P1);
	DECLARE_MW_GAMEPLAY_TAG(Battle_ComboAttack_P2);
	DECLARE_MW_GAMEPLAY_TAG(Battle_ComboAttack_P3);
	// コンボ
	DECLARE_MW_GAMEPLAY_TAG(Battle_ComboCentral);
	DECLARE_MW_GAMEPLAY_TAG(Battle_ComboUpDown);
	DECLARE_MW_GAMEPLAY_TAG(Battle_ComboLeftRight);
	// コンボヒット
	DECLARE_MW_GAMEPLAY_TAG(Battle_ComboHit);
	DECLARE_MW_GAMEPLAY_TAG(Battle_AvatarChanged);

	// キャラクター
	DECLARE_MW_GAMEPLAY_TAG(Character_CombatState);
#pragma endregion


#pragma endregion

	// ==== SetByCallerタグ ====
#pragma region SetByCaller Tag
	DECLARE_MW_GAMEPLAY_TAG(GP_SetByCaller_ComboHitRatio);
#pragma endregion
};
