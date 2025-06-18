#include "Define/MWDefineGameplay.h"

int32 FMWTeam::GetTeamSpeed() const
{
	return 1;
}

FMWTeamUnit FMWTeam::GetLeader() const
{
	for (auto& unit : Units)
	{
		if (unit.bIsLeader)
		{
			return unit;
		}
	}

	return FMWTeamUnit();
}

FMWTeamUnit FMWTeam::GetMember(int Index) const
{
	return FMWTeamUnit();
}