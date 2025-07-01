#pragma once

// Include Header
#include "Subsystems/WorldSubsystem.h"
#include "MWPartyManager.generated.h"

// Forward Declare

// Macro

/*
 * @class UMWPartyManager
 * 
 * @brief The manager of party and teams.
 *
 * @note
 */
UCLASS()
class UMWPartyManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

private:
	struct FMemberInfo
	{
		int32 Id = INDEX_NONE;

		/* No. of the team that the character belongs to.*/
		int32 TeamNo = INDEX_NONE;

		bool bPartyLeader = false;

		bool bTeamLeader = false;

		FMemberInfo() = default;

		FMemberInfo(int32 InId) : Id(InId) {}
 
		bool operator==(const FMemberInfo& Other) const
		{
			return Id == Other.Id && TeamNo == Other.TeamNo;
		}

		int32 GetId() const { return Id; }

		int32 GetTeamNo() const { return TeamNo; }

		bool IsPartyLeader() const { return bPartyLeader; }

		bool IsTeamLeader() const { return bTeamLeader; }

		void SetAsTeamLeader(bool bLeader) { bTeamLeader = bLeader; }

		void SetAsPartyLeader(bool bLeader) { bPartyLeader = bLeader; }

		void Reset();
	};

	struct FTeam
	{
		int32 No = INDEX_NONE;

		TArray<int32> MemberIds;

		int32 LeaderId = INDEX_NONE;

		bool IsInTeam(int32 Id) const;

		bool AddMember(int32 Id);

		bool RemoveMember(int32 Id);

		bool SetLeader(int32 Id);

		bool IsEmpty() const;

		void Empty();

		int32 GetLeader() const { return LeaderId; }

		int32 GetMemberByPos(int32 Pos) const;

		bool HasLeader() const { return LeaderId != INDEX_NONE; }

		int32 GetTeamNo() const { return No; }

		void SetNo(int32 NewNo) { No = NewNo; }

	private:
		void ResetLeaderId() { LeaderId = INDEX_NONE; }
	};

public:
	void Initialize(FSubsystemCollectionBase& Collection) override;

	void Deinitialize() override;

public:
	/* Call this when game starts play. */
	void InitializeParty();

	/* Add a new character into party by character's Id. */
	bool AddCharacterToParty(int32 Id);

	/* Remove a character from party by character's Id. */
	bool RemoveCharacterFromParty(int32 Id);

	/* Set the party leader. */
	bool SetLeaderForParty(int32 Id);

	bool SetMemberToTeam(int32 TeamNo, int32 Id);

	/*
	* Remove the member from specific team.
	* 
	* @ret removed members number
	*/
	int32 RemoveMemberFromTeam(int32 Id);

	/* Directly set the team's leader by given TeamNo. */
	bool SetTeamLeaderByTeamNo(int32 Id, int32 TeamNo);

	/* Set the team's leader. Find the Team by given member's id. */
	bool SetTeamLeader(int32 Id);

	bool IsPartyMember(int32 Id) const;

private:
	bool GetMember(FMemberInfo* OutMember, int32 Id);

	bool GetTeam(FTeam* OutTeam, int32 Id);

	void InitializeTeams();

private:
	int32 PartyLeaderId;

	TArray<FMemberInfo> Members;

	/* Save the team by it's No.*/
	TArray<FTeam> Teams;

	bool bInitialized = false;
};