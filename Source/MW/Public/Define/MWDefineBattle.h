#pragma once

#include "CoreMinimal.h"
#include "MWDefineBattle.generated.h"

UENUM(BlueprintType)
enum class EMWAttackResult : uint8
{
    Normal             UMETA(DisplayName = "Normal"),
    Critical           UMETA(DisplayName = "Critical"),
    Miss               UMETA(DisplayName = "Miss"),
    InvincibleBlocked  UMETA(DisplayName = "Invincible Blocked"),

	Max				   UMETA(Hidden)
};