#include "Gameplay/Battle/MWBattleScenePreparation.h"
#include "Data/MWGameplayData.h"
#include "Data/MWBattleData.h"
#include "EngineUtils.h"
#include "Gameplay/Battle/MWBattleUnitPosition.h"

FMWBattleScenePreparation::FMWBattleScenePreparation()
{
}

FMWBattleScenePreparation::~FMWBattleScenePreparation()
{

}

void FMWBattleScenePreparation::PrepareScene(const FMWBattleSceneParam& Param)
{
	TArray<FMWTeam> enemyTeams, playerTeams;

	for (const auto& team : Param.Teams)
	{
		if (team.GetTeamAlign() == EMWTeamAlign::Enemy)
		{
			enemyTeams.Emplace(team);
		}
		else if (team.GetTeamAlign() == EMWTeamAlign::Player)
		{
			playerTeams.Emplace(team);
		}
	}

	const int32 enemyNo = enemyTeams.Num();
	const int32 playerNo = playerTeams.Num();

	check(enemyNo > 0 && playerNo > 0);

	// sort the enemy teams and player teams by index
	auto teamSort = [](const FMWTeam& A, const FMWTeam& B) -> bool
	{
		return A.TeamNo < B.TeamNo;
	};

	enemyTeams.Sort(teamSort);
	playerTeams.Sort(teamSort);

	UWorld* world = Param.Teams[0].Units[0].Pawn->GetWorld();

	// collect battle unit position and set character to right location
	for (TActorIterator<AMWBattleUnitPosition> iter(world); iter; ++iter)
	{
		AMWBattleUnitPosition* unitPos = *iter;

		if (unitPos->Align == EMWTeamAlign::Enemy)
		{
			for (auto& team : enemyTeams)
			{
				TrySetTeamPositionFromBattleUnitPos(team, unitPos);
			}
		}
		else if (unitPos->Align == EMWTeamAlign::Player)
		{
			for (auto& team : playerTeams)
			{
				TrySetTeamPositionFromBattleUnitPos(team, unitPos);
			}
		}
	}
}

void FMWBattleScenePreparation::TrySetTeamPositionFromBattleUnitPos(FMWTeam& Team, const AMWBattleUnitPosition* BattleUnitPos)
{
	if (Team.Units.Num() != BattleUnitPos->UnitNo)
	{
		return;
	}

	int32 memNo = 0;

	if (Team.TeamNo == BattleUnitPos->Index)
	{
		for (auto& teamUnit : Team.Units)
		{
			check(teamUnit.Pawn != nullptr);

			FVector worldPos;

			if (teamUnit.bIsLeader)
			{
				worldPos = BattleUnitPos->GetTransform().TransformPosition(BattleUnitPos->Position[0]);
			}
			else
			{
				worldPos = BattleUnitPos->GetTransform().TransformPosition(BattleUnitPos->Position[1 + memNo++]);
			}

			teamUnit.Pawn->SetActorLocation(worldPos);

			if (BattleUnitPos->Align == EMWTeamAlign::Enemy)
			{
				teamUnit.Pawn->SetActorRotation(FRotator(0.f, 180.f, 0.f));
			}
		}
	}
}
