#pragma once

#include "NativeGameplayTags.h"

#define DECLARE_UE_GAMEPLAY_TAG(TagName) COMBATRELATION_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TagName)

namespace FactionRelationTags
{
	COMBATRELATION_API FGameplayTag FindTagByString(const FString& TagString, bool bMatchPartialString = false);

	/** The actor in player's team. */
	DECLARE_UE_GAMEPLAY_TAG(CombatRelation_Faction_Player);
	/** The default tag of enemy such as field enemies, wandering monsters. */
	DECLARE_UE_GAMEPLAY_TAG(CombatRelation_Faction_Enemy);
}

#undef DECLARE_UE_GAMEPLAY_TAG