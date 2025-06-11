#pragma once

#include "MWEnum.generated.h"

#pragma region Character
UENUM(BlueprintType)
enum ECharacterBehaviorState
{
	Normal,
	Battle
};
#pragma endregion

#pragma region Battle
UENUM()
enum class EBattleResult
{
	PlayerWin	= 0,
	EnemyWin	= 1,
	Draw		= 2,
	Max			= 3
};

UENUM(BlueprintType)
enum class EMWTeamAlign : uint8
{
	Player,
	Enemy,
	Friendly,
	Neutral
};
#pragma endregion

UENUM(BlueprintType)
enum class EMWBattleActionBuff
{
	Idle,
	Attack,
	Heal
};