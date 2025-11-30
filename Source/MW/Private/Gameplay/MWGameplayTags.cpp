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

#pragma region Action Tag
	// Basic
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_Basic_Direction,				"InputTag.Basic.Direction",								"Basic Input Direction.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_Basic_Confirm,					"InputTag.Basic.Confirm",								"Basic Input Confirm.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_Basic_Cancel,					"InputTag.Basic.Cancel",								"Basic Input Cancel.");

	// TPDefault Input
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_TPDefault_Debug,				"InputTag.TPDefault.Debug",								"Third-Person default action Move Debug.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_TPDefault_Move,				"InputTag.TPDefault.Move",								"Third-Person default action Move.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_TPDefault_LookAt,				"InputTag.TPDefault.LookAt",							"Third-Person default action LookAt.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_TPDefault_AutoRun,				"InputTag.TPDefault.AutoRun",							"Third-Person default action Auto-run.");

	// Battle Command
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_BC_Move,						"InputTag.BC.Move",										"Input Battle Command Move.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_BC_ChangeLeader,				"InputTag.BC.ChangeLeader",								"Input Battle Command ChangeLeader.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_BC_UseItem,					"InputTag.BC.Item",										"Input Battle Command Item.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_BC_UseSpirit,					"InputTag.BC.Spirit",									"Input Battle Command Spirit.");

	// Move Command
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_BC_Move_C1,					"InputTag.BC.Move.C1",									"Input Battle Command Move Character1 Action.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_BC_Move_C1U,					"InputTag.BC.Move.C1U",									"Input Battle Command Move Character1 Action Combo Up.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_BC_Move_C1D,					"InputTag.BC.Move.C1D",									"Input Battle Command Move Character1 Action Combo Down.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_BC_Move_C1L,					"InputTag.BC.Move.C1L",									"Input Battle Command Move Character1 Action Combo Left.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_BC_Move_C1R,					"InputTag.BC.Move.C1R",									"Input Battle Command Move Character1 Action Combo Right.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_BC_Move_C2,					"InputTag.BC.Move.C2",									"Input Battle Command Move Character2 Action.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_BC_Move_C2U,					"InputTag.BC.Move.C2U",									"Input Battle Command Move Character2 Action Combo Up.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_BC_Move_C2D,					"InputTag.BC.Move.C2D",									"Input Battle Command Move Character2 Action Combo Down.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_BC_Move_C2L,					"InputTag.BC.Move.C2L",									"Input Battle Command Move Character2 Action Combo Left.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_BC_Move_C2R,					"InputTag.BC.Move.C2R",									"Input Battle Command Move Character2 Action Combo Right.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_BC_Move_C3,					"InputTag.BC.Move.C3",									"Input Battle Command Move Character3 Action.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_BC_Move_C3U,					"InputTag.BC.Move.C3U",									"Input Battle Command Move Character3 Action Combo Up.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_BC_Move_C3D,					"InputTag.BC.Move.C3D",									"Input Battle Command Move Character3 Action Combo Down.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_BC_Move_C3L,					"InputTag.BC.Move.C3L",									"Input Battle Command Move Character3 Action Combo Left.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_BC_Move_C3R,					"InputTag.BC.Move.C3R",									"Input Battle Command Move Character3 Action Combo Right.");
#pragma endregion

#pragma region Input Mapping Tag
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IMC_Basic,							"InputTag.MappingContext.Basic",						"Input Mapping Context Basic.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IMC_TPDefault,						"InputTag.MappingContext.TPDefault",					"Input Mapping Context Third-Person-Default.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IMC_BattleCommand,					"InputTag.MappingContext.BattleCommand",				"Input Mapping Context Battle Command.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IMC_BattleCharacterAction,			"InputTag.MappingContext.BattleCharacterAction",		"Input Mapping Context Character Action in Battle.");
#pragma endregion

#pragma region Battle Tag
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GP_Battle_Ability_UnitAttack,		"GameplayTag.Battle.Ability.UnitAttack",				"Activate UMWBattleUnitAttack ability.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GP_Battle_ComboAttack_P1,			"GameplayTag.Battle.ComboAttack.P1",					"Player 1 Battle Combo Attack.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GP_Battle_ComboAttack_P2,			"GameplayTag.Battle.ComboAttack.P2",					"Player 2 Battle Combo Attack.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GP_Battle_ComboAttack_P3,			"GameplayTag.Battle.ComboAttack.P3",					"Player 3 Battle Combo Attack.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GP_Battle_ComboCentral,				"GameplayTag.Battle.ComboCentral",						"Battle Combo Central.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GP_Battle_ComboUpDown,				"GameplayTag.Battle.ComboUpDown",						"Battle Combo ComboUpDown.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GP_Battle_ComboLeftRight,			"GameplayTag.Battle.ComboLeftRight",					"Battle Combo ComboLeftRight.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GP_Battle_ComboHit,					"GameplayTag.Battle.ComboHit",							"Battle Combo Hit.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GP_Battle_AvatarChanged,				"GameplayTag.Battle.AvatarChanged",						"Battle Avatar changed / swapped event.");
#pragma endregion

#pragma region SetByCaller Tag
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GP_SetByCaller_ComboHitRatio,		"GameplayTag.SetByCaller.ComboHitRatio",			"SetByCaller : Combo Hit Ratio.");
#pragma endregion

	TArray<FGameplayTag> BattleCommands = 
	{
		MWGameplayTags::IATag_BC_Move,
		MWGameplayTags::IATag_BC_ChangeLeader,
		MWGameplayTags::IATag_BC_UseItem,
		MWGameplayTags::IATag_BC_UseSpirit,
	};

	TArray<FGameplayTag> BattleCharacterAction =
	{
		MWGameplayTags::IATag_BC_Move_C1,
		MWGameplayTags::IATag_BC_Move_C1U,
		MWGameplayTags::IATag_BC_Move_C1D,
		MWGameplayTags::IATag_BC_Move_C1L,
		MWGameplayTags::IATag_BC_Move_C1R,
		MWGameplayTags::IATag_BC_Move_C2,
		MWGameplayTags::IATag_BC_Move_C2U,
		MWGameplayTags::IATag_BC_Move_C2D,
		MWGameplayTags::IATag_BC_Move_C2L,
		MWGameplayTags::IATag_BC_Move_C2R,
		MWGameplayTags::IATag_BC_Move_C3,
		MWGameplayTags::IATag_BC_Move_C3U,
		MWGameplayTags::IATag_BC_Move_C3D,
		MWGameplayTags::IATag_BC_Move_C3L,
		MWGameplayTags::IATag_BC_Move_C3R
	};
}

