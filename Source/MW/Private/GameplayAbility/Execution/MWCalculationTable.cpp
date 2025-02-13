#include "GameplayAbility/Execution/MWCalculationTable.h"

float MWCalculationTable::DamageCalulation(const float Atk, const float Def)
{
	return FMath::Max(Atk - Def, 0.f);
}

