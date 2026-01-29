// Copyright Epic Games, Inc. All Rights Reserved.

#include "Gameplay/MWGameplayTags.h"

#include "Engine/EngineTypes.h"
#include "GameplayTagsManager.h"
#include "MWLogChannels.h"

namespace MWGameplayTags
{
	// Input Mapping Context Tag
	FGameplayTag FindTagByString(const FString& TagString, bool bMatchPartialString)
	{
		const UGameplayTagsManager& Manager = UGameplayTagsManager::Get();
		FGameplayTag Tag = Manager.RequestGameplayTag(FName(*TagString), false);

		if (!Tag.IsValid() && bMatchPartialString)
		{
			FGameplayTagContainer AllTags;
			Manager.RequestAllGameplayTags(AllTags, true);

			for (const FGameplayTag& TestTag : AllTags)
			{
				if (TestTag.ToString().Contains(TagString))
				{
					UE_LOG(LogMW, Display, TEXT("Could not find exact match for tag [%s] but found partial match on tag [%s]."), *TagString, *TestTag.ToString());
					Tag = TestTag;
					break;
				}
			}
		}

		return Tag;
	}

	// ==== Basic ====
#pragma region Basic
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IMC_Basic,							"InputTag.MappingContext.Basic",						"Input Mapping Context Basic.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_Basic_Direction,				"InputTag.Basic.Direction",								"Basic Input Direction.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_Basic_Confirm,					"InputTag.Basic.Confirm",								"Basic Input Confirm.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_Basic_Cancel,					"InputTag.Basic.Cancel",								"Basic Input Cancel.");
#pragma endregion

	// ==== TPDefault Input ====
#pragma region TPDefault
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IMC_TPDefault,						"InputTag.MappingContext.TPDefault",					"Input Mapping Context Third-Person-Default.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_TPDefault_Debug,				"InputTag.TPDefault.Debug",								"Third-Person default action Move Debug.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_TPDefault_Move,				"InputTag.TPDefault.Move",								"Third-Person default action Move.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_TPDefault_LookAt,				"InputTag.TPDefault.LookAt",							"Third-Person default action LookAt.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_TPDefault_AutoRun,				"InputTag.TPDefault.AutoRun",							"Third-Person default action Auto-run.");
#pragma endregion

	// ==== Battle Command ====
#pragma region Battle Command

	// == General ==
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IMC_BattleCommand,					"InputTag.MappingContext.BattleCommand",				"Input Mapping Context Battle Command.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_BC_Attack,						"InputTag.BC.Attack",									"Input Battle Command Attack.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_BC_ChangeAvatar,				"InputTag.BC.ChangeAvatar",								"Input Battle Command Change Avatar.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_BC_UseItem,					"InputTag.BC.UseItem",									"Input Battle Command Use Item.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_BC_UseSpirit,					"InputTag.BC.UseSpirit",								"Input Battle Command Use Spirit.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_BC_Escape,						"InputTag.BC.Escape",									"Input Battle Command Escape.");

	// == Attack ==
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IMC_BattleCommand_Attack,			"InputTag.MappingContext.BattleCommand.Attack",			"Input Mapping Context Battle Command Attack.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_BC_Attack_SelectTarget,		"InputTag.BC.Attack.SelectTarget",						"Battle Command Attack SelectTarget.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_BC_Attack_Cancel,				"InputTag.BC.Attack.Cancel",							"Battle Command Attack Cancel.");

	// Character 1 control.
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_BC_Attack_C1,					"InputTag.BC.Attack.C1",								"Input Battle Command Attack Character1 Action.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_BC_Attack_C1UD,				"InputTag.BC.Attack.C1UD",								"Input Battle Command Attack Character1 Action Combo Up Down.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_BC_Attack_C1LR,				"InputTag.BC.Attack.C1LR",								"Input Battle Command Attack Character1 Action Combo Left Right.");
	// Character 2 control.
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_BC_Attack_C2,					"InputTag.BC.Attack.C2",								"Input Battle Command Attack Character2 Action.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_BC_Attack_C2UD,				"InputTag.BC.Attack.C2UD",								"Input Battle Command Attack Character2 Action Combo Up Down.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_BC_Attack_C2LR,				"InputTag.BC.Attack.C2LR",								"Input Battle Command Attack Character2 Action Combo Left Right.");
	// Character 3 control.
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_BC_Attack_C3,					"InputTag.BC.Attack.C3",								"Input Battle Command Attack Character3 Action.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_BC_Attack_C3UD,				"InputTag.BC.Attack.C3UD",								"Input Battle Command Attack Character3 Action Combo Up Down.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_BC_Attack_C3LR,				"InputTag.BC.Attack.C3LR",								"Input Battle Command Attack Character3 Action Combo Left Right.");

	// Character action tags container.
	TArray<FGameplayTag> BattleCharacterAttack =
	{
		MWGameplayTags::IATag_BC_Attack_C1,
		MWGameplayTags::IATag_BC_Attack_C1UD,
		MWGameplayTags::IATag_BC_Attack_C1LR,
		MWGameplayTags::IATag_BC_Attack_C2,
		MWGameplayTags::IATag_BC_Attack_C2UD,
		MWGameplayTags::IATag_BC_Attack_C2LR,
		MWGameplayTags::IATag_BC_Attack_C3,
		MWGameplayTags::IATag_BC_Attack_C3UD,
		MWGameplayTags::IATag_BC_Attack_C3LR
	};

	// == ChangeAvatar ==
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IMC_BattleCommand_ChangeAvatar,		"InputTag.MappingContext.BattleCommand.ChangeAvatar",	"Input Mapping Context Battle Command Change Avatar.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_BC_ChangeAvatar_Confirm,		"InputTag.BC.ChangeAvatar.Confirm",						"Battle Command ChangeAvatar Confirm.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_BC_ChangeAvatar_Cancel,		"InputTag.BC.ChangeAvatar.Cancel",						"Battle Command ChangeAvatar Cancel.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_BC_ChangeAvatar_SelectTarget,	"InputTag.BC.ChangeAvatar.SelectTarget",				"Battle Command ChangeAvatar SelectTarget.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_BC_ChangeAvatar_ChangeAvatar,	"InputTag.BC.ChangeAvatar.ChangeAvatar",				"Battle Command ChangeAvatar ChangeAvatar.");

	// == UseItem ==
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IMC_BattleCommand_UseItem,			"InputTag.MappingContext.BattleCommand.UseItem",		"Input Mapping Context Battle Command Use Item.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_BC_UseItem_Confirm,			"InputTag.BC.UseItem.Confirm",							"Battle Command UseItem Confirm.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_BC_UseItem_Cancel,				"InputTag.BC.UseItem.Cancel",							"Battle Command UseItem Cancel.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_BC_UseItem_SelectItem,			"InputTag.BC.UseItem.SelectItem",						"Battle Command UseItem SelectItem.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_BC_UseItem_SelectTarget,		"InputTag.BC.UseItem.SelectTarget",						"Battle Command UseItem SelectTarget.");

	// == UseSprit ==
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IMC_BattleCommand_UseSpirit,			"InputTag.MappingContext.BattleCommand.UseSpirit",		"Input Mapping Context Battle Command Use Spirit.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_BC_UseSpirit_Confirm,			"InputTag.BC.UseSpirit.Confirm",						"Battle Command UseSpirit Confirm.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_BC_UseSpirit_Cancel,			"InputTag.BC.UseSpirit.Cancel",							"Battle Command UseSpirit Cancel.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_BC_UseSpirit_SelectSpirit,		"InputTag.BC.UseSpirit.SelectSpirit",					"Battle Command UseSpirit SelectSpirit.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_BC_UseSpirit_SelectTarget,		"InputTag.BC.UseSpirit.SelectTarget",					"Battle Command UseSpirit SelectTarget.");

	// Battle Command tags container.
	TArray<FGameplayTag> BattleCommands =
	{
		MWGameplayTags::IATag_BC_Attack,
		MWGameplayTags::IATag_BC_ChangeAvatar,
		MWGameplayTags::IATag_BC_UseItem,
		MWGameplayTags::IATag_BC_UseSpirit,
		MWGameplayTags::IATag_BC_Escape
	};
#pragma endregion

	// ==== Gameplay Tag ====
#pragma region Gameplay Tag
	// Gameplay Ability
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_ActivateFail_ActivationGroup,	"Ability.ActivateFail.ActivationGroup",					"Ability failed to activate because of its activation group.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Input_Charging,					"Ability.Input.Charging",								"Ability's Input Charging.");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GP_Battle_Ability_UnitAttack,			"GameplayTag.Battle.Ability.UnitAttack",				"Activate UMWBattleUnitAttack ability.");
	// Attack command
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GP_Battle_ComboAttack_P1,				"GameplayTag.Battle.ComboAttack.P1",					"Player 1 Battle Combo Attack.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GP_Battle_ComboAttack_P2,				"GameplayTag.Battle.ComboAttack.P2",					"Player 2 Battle Combo Attack.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GP_Battle_ComboAttack_P3,				"GameplayTag.Battle.ComboAttack.P3",					"Player 3 Battle Combo Attack.");
	// Combo
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GP_Battle_ComboCentral,					"GameplayTag.Battle.ComboCentral",						"Battle Combo Central.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GP_Battle_ComboUpDown,					"GameplayTag.Battle.ComboUpDown",						"Battle Combo ComboUpDown.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GP_Battle_ComboLeftRight,				"GameplayTag.Battle.ComboLeftRight",					"Battle Combo ComboLeftRight.");
	// Combo Hit
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GP_Battle_ComboHit,						"GameplayTag.Battle.ComboHit",							"Battle Combo Hit.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GP_Battle_AvatarChanged,					"GameplayTag.Battle.AvatarChanged",						"Battle Avatar changed / swapped event.");
	
	// Character
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GP_Character_CombatState,				"GameplayTag.Character.CombatState",					"Character combat state.");
#pragma endregion

	// ==== SetByCaller Tag ====
#pragma region SetByCaller Tag
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GP_SetByCaller_ComboHitRatio,			"GameplayTag.SetByCaller.ComboHitRatio",				"SetByCaller : Combo Hit Ratio.");
#pragma endregion
}

