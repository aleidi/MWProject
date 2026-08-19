// ============================================================================
//  CombatRelationの型
// ============================================================================
#pragma once

#include "CoreMinimal.h"
#include "CombatRelationTypes.generated.h"

/**
 * ECombatRelation
 * 敵対／友好関係の列挙型（「自分」の視点）。
 */
UENUM(BlueprintType)
enum class ECombatRelation : uint8
{
    Friendly UMETA(DisplayName="Friendly"),   // 味方
    Hostile  UMETA(DisplayName="Hostile"),    // 敵対
    Neutral  UMETA(DisplayName="Neutral"),    // 中立
    Ignore   UMETA(DisplayName="Ignore")      // 無視（自分／無効）
};

/**
 * ETargetPolicy
 * 高レベルのターゲットポリシー（スキル／問い合わせのフィルタリングに使用）。
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
