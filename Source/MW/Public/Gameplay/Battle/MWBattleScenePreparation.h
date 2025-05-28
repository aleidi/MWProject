#pragma once

#include "CoreMinimal.h"
#include "Define/MWStruct.h"

class FMWBattleScenePreparation : public TSharedFromThis<FMWBattleScenePreparation>
{
public:
	FMWBattleScenePreparation();
	~FMWBattleScenePreparation();

	void PrepareScene(const FMWBattleSceneParam& Param);

private:
	void TrySetTeamPositionFromBattleUnitPos(FMWTeam& Team, const class AMWBattleUnitPosition* BattleUnitPos);

	class UMWBattleSceneSetting* Setting;

	/* The origin of the battle scene. */
	FVector Origin;

	int32 CurrEnemyId;
	int32 CurrFriendlyId;

};