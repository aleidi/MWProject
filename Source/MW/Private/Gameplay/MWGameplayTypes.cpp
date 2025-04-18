#include "Gameplay/MWGameplayTypes.h"

int32 FMWTeam::GetTeamSpeed() const
{
	return 1;
}

FMWTeamUnit FMWTeam::GetLeader() const
{
	return FMWTeamUnit();
}

FMWTeamUnit FMWTeam::GetMember(int Index) const
{
	return FMWTeamUnit();
}