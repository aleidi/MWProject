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

	// Basic
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_Basic_DirectionalInput,		"InputTag.Basic.DirectionalInput",				"Basic Input DirectionalInput.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_Basic_Confirm,					"InputTag.Basic.Confirm",						"Basic Input Confirm.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_Basic_Cancel,					"InputTag.Basic.Cancel",						"Basic Input Cancel.");

	// Default Input
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_Default_Debug,					"InputTag.Default.Debug",						"Move Debug.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_Default_Move,					"InputTag.Default.Move",						"Move input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_Default_LookAt,				"InputTag.Default.LookAt",						"LookAt input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_Default_AutoRun,				"InputTag.Default.AutoRun",						"Auto-run input.");

	// Battle Command
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_BC_Move,						"InputTag.BC.Move",								"Input Battle Command Move.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_BC_ChangeLeader,				"InputTag.BC.ChangeLeader",						"Input Battle Command ChangeLeader.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_BC_UseItem,					"InputTag.BC.Item",								"Input Battle Command Item.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_BC_UseSpirit,					"InputTag.BC.Spirit",							"Input Battle Command Spirit.");

	// Combat Command
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_CC_Attack_Up,					"InputTag.CC.Attack.Up",						"Input CombatCommand Attack Up.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_CC_Attack_Down,				"InputTag.CC.Attack.Down",						"Input CombatCommand Attack Down.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_CC_Attack_Left,				"InputTag.CC.Attack.Left",						"Input CombatCommand Attack Left.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_CC_Attack_Right,				"InputTag.CC.Attack.Right",						"Input CombatCommand Attack Right.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_CC_Dir_Up,						"InputTag.CC.Dir.Up",							"Input CombatCommand Direction Up.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_CC_Dir_Down,					"InputTag.CC.Dir.Down",							"Input CombatCommand Direction Down.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_CC_Dir_Left,					"InputTag.CC.Dir.Left",							"Input CombatCommand Direction Left.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_CC_Dir_Right,					"InputTag.CC.Dir.Right",						"Input CombatCommand Direction Right.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_CC_SupportAttack1,				"InputTag.CC.SupportAttack1",					"Input CombatCommand SupportAttack1.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_CC_SupportAttack2,				"InputTag.CC.SupportAttack2",					"Input CombatCommand SupportAttack2.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IATag_CC_UltimateSkill,				"InputTag.CC.UltimateSkill",					"Input CombatCommand UltimateSkill.");

#pragma region InputMappingTag
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IMCTag_Basic,						"InputTag.MappingContext.Basic",				"Input Mapping Context Basic.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IMCTag_BattleCommand,				"InputTag.MappingContext.BattleCommand",		"Input Mapping Context Battle Command.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IMCTag_CombatCommand,				"InputTag.MappingContext.UnitCommand",			"Input Mapping Context Unit Command.");
#pragma endregion
}

