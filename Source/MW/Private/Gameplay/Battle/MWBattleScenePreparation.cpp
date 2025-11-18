#include "Gameplay/Battle/MWBattleScenePreparation.h"
#include "Data/MWGameplayData.h"
#include "Data/MWBattleData.h"
#include "EngineUtils.h"
#include "Gameplay/Battle/MWBattleUnitPosition.h"
#include "Gameplay/Battle/BattleUnit/MWBattleUnitAvatar.h"

FMWBattleScenePreparation::FMWBattleScenePreparation()
{
}

FMWBattleScenePreparation::~FMWBattleScenePreparation()
{

}

void FMWBattleScenePreparation::PrepareScene(const FMWBattleSceneParam& Param)
{
	FMWTeam playerTeam = Param.PlayerTeam;
	FMWTeam enemyTeam = Param.EnemyTeam;

	const int32 enemyNo = playerTeam.BattleUnits.Num();
	const int32 playerNo = enemyTeam.BattleUnits.Num();

	check(enemyNo > 0 && playerNo > 0);

	UWorld* world = playerTeam.BattleUnits[0]->GetWorld();

	// collect battle unit position and set character to right location
	for (TActorIterator<AMWBattleUnitPosition> iter(world); iter; ++iter)
	{
		AMWBattleUnitPosition* unitPos = *iter;

		if (unitPos->Align == EMWTeamAlign::Enemy)
		{
			TrySetTeamPositionFromBattleUnitPos(enemyTeam, unitPos);
		}
		else if (unitPos->Align == EMWTeamAlign::Player)
		{
			TrySetTeamPositionFromBattleUnitPos(playerTeam, unitPos);
		}
	}
}

void FMWBattleScenePreparation::TrySetTeamPositionFromBattleUnitPos(FMWTeam& Team, const AMWBattleUnitPosition* BattleUnitPos)
{
	if (Team.BattleUnits.Num() != BattleUnitPos->UnitNo)
	{
		return;
	}

	int32 memNo = 0;

	if (BattleUnitPos->Index < Team.BattleUnits.Num())
	{
		FVector worldPos;

		worldPos = BattleUnitPos->GetTransform().TransformPosition(BattleUnitPos->Position[0]);

		//if (teamUnit.bIsLeader)
		//{
		//	worldPos = BattleUnitPos->GetTransform().TransformPosition(BattleUnitPos->Position[0]);
		//}
		//else
		//{
		//	worldPos = BattleUnitPos->GetTransform().TransformPosition(BattleUnitPos->Position[1 + memNo++]);
		//}

		auto* actor = Team.BattleUnits[BattleUnitPos->Index].Get();

		actor->SetActorLocation(worldPos);

		if (BattleUnitPos->Align == EMWTeamAlign::Enemy)
		{
			actor->SetActorRotation(FRotator(0.f, 180.f, 0.f));
		}
	}
}
