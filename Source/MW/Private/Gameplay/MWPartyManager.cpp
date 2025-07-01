#include "Gameplay/MWPartyManager.h"
#include "System/MWAssetManager.h"
#include "Data/MWMasterData.h"

void UMWPartyManager::FMemberInfo::Reset()
{
	Id				= INDEX_NONE;
	TeamNo			= INDEX_NONE;
	bPartyLeader	= false;
	bTeamLeader		= false;
}

bool UMWPartyManager::FTeam::IsInTeam(int32 Id) const
{
	return MemberIds.Contains(Id);
}

bool UMWPartyManager::FTeam::AddMember(int32 Id)
{
	if (IsInTeam(Id))
	{
		return false;
	}

	MemberIds.Emplace(Id);

	return true;
}

bool UMWPartyManager::FTeam::RemoveMember(int32 Id)
{
	if(!IsInTeam(Id))
	{
		return false;
	}
	
	auto res = MemberIds.Remove(Id);

	if (res > 0)
	{
		if (GetLeader() == Id)
		{
			ResetLeaderId();
		}

		return true;
	}

	return false;
}

bool UMWPartyManager::FTeam::SetLeader(int32 Id)
{
	if (!IsInTeam(Id))
	{
		return false;
	}

	LeaderId = Id;

	return true;
}

bool UMWPartyManager::FTeam::IsEmpty() const
{
	return MemberIds.Num() == 0;
}

void UMWPartyManager::FTeam::Empty()
{
	MemberIds.Reset();
}

int32 UMWPartyManager::FTeam::GetMemberByPos(int32 Pos) const
{
	if (!IsEmpty() && FMath::IsWithin(Pos, 0, MemberIds.Num()))
	{
		return MemberIds[Pos];
	}

	return INDEX_NONE;
}

void UMWPartyManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UMWPartyManager::Deinitialize()
{
	Super::Deinitialize();
}

void UMWPartyManager::InitializeParty()
{
	if (bInitialized)
	{
		return;
	}

	InitializeTeams();

	bInitialized = true;
}

bool UMWPartyManager::AddCharacterToParty(int32 Id)
{
	if (Members.Contains(Id))
	{
		return false;
	}

	Members.Emplace(FMemberInfo(Id));

	return true;
}

bool UMWPartyManager::RemoveCharacterFromParty(int32 Id)
{
	if (!Members.Contains(Id))
	{
		return false;
	}

	// remove from team first
	RemoveMemberFromTeam(Id);

	// then remove from party
	Members.RemoveAll(
	[Id](const FMemberInfo& A)
	{
		return A.Id == Id;
	});

	return false;
}

bool UMWPartyManager::SetLeaderForParty(int32 Id)
{
	bool res = false;

	for (auto& member : Members)
	{
		if (member.Id == Id)
		{
			PartyLeaderId = Id;

			member.bPartyLeader = true;

			res = true;

			continue;
		}

		if (true == member.bPartyLeader && member.Id != Id)
		{
			member.bPartyLeader = false;
		}
	}

	return res;
}

bool UMWPartyManager::SetMemberToTeam(int32 TeamNo, int32 Id)
{
	FMemberInfo* member = nullptr;

	// check the member
	GetMember(member, Id);

	if (!member)
	{
		return false;
	}

	bool res = false;

	for (auto& team : Teams)
	{
		if (team.GetTeamNo() == TeamNo)
		{
			// if team is empty, then set the new member as team leader
			bool bShouldSetAsLeader = team.IsEmpty();

			res = team.AddMember(Id);
			
			if(res && bShouldSetAsLeader)
			{
				team.SetLeader(Id);
			}

			break;
		}
	}

	return res;
}

int32 UMWPartyManager::RemoveMemberFromTeam(int32 Id)
{
	int32 noRemove = 0;

	for (auto& team : Teams)
	{
		noRemove += team.RemoveMember(Id) ? 1 : 0;

		break;
	}

	return noRemove;
}

bool UMWPartyManager::SetTeamLeaderByTeamNo(int32 Id, int32 TeamNo)
{
	bool bleaderChange = false;

	// change the leader info of team
	for (auto& team : Teams)
	{
		if (team.GetTeamNo() == TeamNo)
		{
			bleaderChange = team.SetLeader(Id);

			break;
		}
	}

	if (!bleaderChange)
	{
		return false;
	}

	for (auto& member : Members)
	{
		// find the team
		if (member.GetTeamNo() == TeamNo)
		{
			// change the leader info of member
			if (member.GetId() == Id)
			{
				member.SetAsTeamLeader(true);
			}
			else if (member.GetId() != Id)
			{
				member.SetAsTeamLeader(false);
			}
		}
	}

	return true;
}

bool UMWPartyManager::SetTeamLeader(int32 Id)
{
	FTeam* findTeam = nullptr;

	GetTeam(findTeam, Id);

	if (!findTeam)
	{
		return false;
	}

	SetTeamLeaderByTeamNo(Id, findTeam->GetTeamNo());

	return true;
}

bool UMWPartyManager::IsPartyMember(int32 Id) const
{
	for (auto member : Members)
	{
		if (member.Id == Id)
		{
			return true;
		}
	}

	return false;
}

bool UMWPartyManager::GetMember(FMemberInfo* OutMember, int32 Id)
{
	for (auto& member : Members)
	{
		if (member.GetId() == Id)
		{
			OutMember = &member;

			return true;
		}
	}

	return false;
}

bool UMWPartyManager::GetTeam(FTeam* OutTeam, int32 Id)
{
	for (auto& team : Teams)
	{
		if (team.IsInTeam(Id))
		{
			OutTeam = &team;

			return true;
		}
	}

	return false;
}

void UMWPartyManager::InitializeTeams()
{
	Teams.Reset();

	auto& data = UMWAssetManager::Get().GetMasterData();

	for (int32 i = 0; i < data.TeamsNumber; ++i)
	{
		FTeam team;
		
		team.SetNo(i);

		Teams.Emplace(team);
	}
}
