#include "Define/MWDefineGameplay.h"

bool FMWTeam::IsAlive() const
{
	return true;
}

EMWTeamAlign FMWTeam::GetTeamAlign() const
{
	return Align;
}

bool FMWTeam::IsValid() const
{
	return BattleUnits.Num() > 0;
}

bool FMWTeam::operator==(const FMWTeam& Other)
{
	return TeamId == Other.TeamId;
}

bool operator==(const FMWTeam& Lhs, const FMWTeam& Rhs)
{
	return Lhs.TeamId == Rhs.TeamId;
}
