#pragma once

#include "NativeGameplayTags.h"

#define DECLARE_UE_GAMEPLAY_TAG(TagName) COMBATRELATION_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TagName)

namespace FactionRelationTags
{
	COMBATRELATION_API FGameplayTag FindTagByString(const FString& TagString, bool bMatchPartialString = false);

	/** プレイヤーのチームに所属するアクター。 */
	DECLARE_UE_GAMEPLAY_TAG(CombatRelation_Faction_Player);
	/** フィールドの敵や徘徊モンスターなどに使用するデフォルトのタグ。 */
	DECLARE_UE_GAMEPLAY_TAG(CombatRelation_Faction_Enemy);
}

#undef DECLARE_UE_GAMEPLAY_TAG