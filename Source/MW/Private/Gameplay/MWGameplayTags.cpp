// Copyright Epic Games, Inc. All Rights Reserved.

#include "Gameplay/MWGameplayTags.h"

#include "Engine/EngineTypes.h"
#include "GameplayTagsManager.h"
#include "MWLogChannels.h"

namespace MWGameplayTags
{
	// Default Input
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Debug, "InputTag.Debug", "Move Debug.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Move, "InputTag.Move", "Move input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_LookAt, "InputTag.LookAt", "LookAt input.");
	//UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Look_Stick, "InputTag.Look.Stick", "Look (stick) input.");
	//UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Crouch, "InputTag.Crouch", "Crouch input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_AutoRun, "InputTag.AutoRun", "Auto-run input.");

	// Battle Command
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_CMD_Attack, "InputTag.CMD.Attack", "Input Command Attack.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_CMD_ChangeLeader, "InputTag.CMD.ChangeLeader", "Input Command ChangeLeader.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_CMD_Charge, "InputTag.CMD.Charge", "Input Command Charge.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_CMD_Item, "InputTag.CMD.Item", "Input Command Item.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_CMD_Special, "InputTag.CMD.Special", "Input Command Special.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_CMD_Spirit, "InputTag.CMD.Spirit", "Input Command Spirit.");

	// Combat Command
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_CC_Attack, "InputTag.CC.Attack", "Input CombatCommand Attack.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_CC_Direction, "InputTag.CC.Direction", "Input CombatCommand Direction.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_CC_SupportAttack1, "InputTag.CC.SupportAttack1", "Input CombatCommand SupportAttack1.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_CC_SupportAttack2, "InputTag.CC.SupportAttack2", "Input CombatCommand SupportAttack2.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_CC_UltimateSkill, "InputTag.CC.UltimateSkill", "Input CombatCommand UltimateSkill.");


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
}

