// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "NativeGameplayTags.h"

namespace MWGameplayTags
{
	MW_API	FGameplayTag FindTagByString(const FString& TagString, bool bMatchPartialString = false);

	// Default Input
	MW_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Debug);
	MW_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Move);
	MW_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_LookAt);
	//MW_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look_Stick);
	//MW_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Crouch);
	MW_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_AutoRun);

	// Battle Command
	MW_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_CMD_Attack);
	MW_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_CMD_ChangeLeader);
	MW_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_CMD_Charge);
	MW_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_CMD_Item);
	MW_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_CMD_Special);
	MW_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_CMD_Spirit);

	// Combat Command
	MW_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_CC_Attack);
	MW_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_CC_Direction);
	MW_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_CC_SupportAttack1);
	MW_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_CC_SupportAttack2);
	MW_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_CC_UltimateSkill);

};
