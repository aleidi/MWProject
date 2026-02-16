// ============================================================================
//  CombatRelation Types
// ============================================================================
#pragma once

#include "CoreMinimal.h"
#include "CombatRelationTypes.generated.h"

/**
 * ECombatRelation
 * Enemy/Friendly relation enum (from "self" perspective).
 */
UENUM(BlueprintType)
enum class ECombatRelation : uint8
{
    Friendly UMETA(DisplayName="Friendly"),   // Friendly
    Hostile  UMETA(DisplayName="Hostile"),    // Hostile
    Neutral  UMETA(DisplayName="Neutral"),    // Neutral
    Ignore   UMETA(DisplayName="Ignore")      // Ignore (Self/Invalid)
};

/**
 * ETargetPolicy
 * High-level target policy (used for skill/query filtering).
 */
UENUM(BlueprintType)
enum class ETargetPolicy : uint8
{
    HostileOnly,
    FriendlyOnly,
    NeutralOnly,
    AnyExceptIgnore,
    SelfOnly,
    PartyOnly,
    FactionFriendlyOnly
};
