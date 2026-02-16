#include "FactionRelationDataAsset.h"

#include "FactionRelationTags.h"

ECombatRelation UFactionRelationDataAsset::Query(const FGameplayTag& SelfFaction, const FGameplayTag& OtherFaction) const
{
    if (!SelfFaction.IsValid() || !OtherFaction.IsValid())
    {
        return ECombatRelation::Neutral;
    }

    if (const FFactionRelationRow* row = Table.Find(SelfFaction))
    {
        if (const ECombatRelation* found = row->Relations.Find(OtherFaction))
        {
            return *found;
        }
    }

    return ECombatRelation::Neutral;
}

void UFactionRelationDataAsset::InitializeDefaultFactions()
{
    FGameplayTag playerFactionTag = FactionRelationTags::CombatRelation_Faction_Player;
    FGameplayTag enemyFactionTag = FactionRelationTags::CombatRelation_Faction_Enemy;

    if (playerFactionTag.IsValid() && enemyFactionTag.IsValid())
    {
        FFactionRelationRow playerRow;
        playerRow.Relations.Add(playerFactionTag, ECombatRelation::Friendly);
        playerRow.Relations.Add(enemyFactionTag, ECombatRelation::Hostile);
        Table.Add(playerFactionTag, playerRow);

        FFactionRelationRow enemyRow;
        enemyRow.Relations.Add(playerFactionTag, ECombatRelation::Hostile);
        enemyRow.Relations.Add(enemyFactionTag, ECombatRelation::Friendly);
        Table.Add(enemyFactionTag, enemyRow);
    }
}
