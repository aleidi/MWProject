#include "FactionRelationTags.h"

namespace FactionRelationTags
{
	FGameplayTag FindTagByString(const FString& TagString, bool bMatchPartialString)
	{
		const UGameplayTagsManager& manager = UGameplayTagsManager::Get();
		FGameplayTag tag = manager.RequestGameplayTag(FName(*TagString), false);

		if (!tag.IsValid() && bMatchPartialString)
		{
			FGameplayTagContainer allTags;
			manager.RequestAllGameplayTags(allTags, true);

			for (const FGameplayTag& testTag : allTags)
			{
				if (testTag.ToString().Contains(TagString))
				{
					UE_LOG(LogTemp, Display, TEXT("Could not find exact match for tag [%s] but found partial match on tag [%s]."), *TagString, *testTag.ToString());
					tag = testTag;
					break;
				}
			}
		}

		return tag;
	}

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(CombatRelation_Faction_Player,		"CombatRelation.Faction.Player",		"Default player faction.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(CombatRelation_Faction_Enemy,		"CombatRelation.Faction.Enemy",			"Default enemy faction.");
}