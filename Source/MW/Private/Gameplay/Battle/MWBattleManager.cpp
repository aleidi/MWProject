#include "Gameplay/Battle/MWBattleManager.h"
#include "Define/MWStruct.h"
#include "Common3DCameraComponent.h"
#include "GameplayTagContainer.h"
#include "Gameplay/Battle/MWBattle.h"
#include "Kismet/GameplayStatics.h"

void FMWBattleManager::SetCharacterCameraAsMain(UMWBattle& Context)
{
	const FMWTeam& team = Context.GetCurrentActionTeam();

	const FMWTeamUnit& leader = team.GetLeader();

	if (leader.Pawn == nullptr)
	{
		return;
	}

	if (auto* pc = UGameplayStatics::GetPlayerController(Context.GetWorld(), 0))
	{
		//pc->SetViewTargetWithBlend(leader.Pawn);
		if (auto* camComp = leader.Pawn->FindComponentByClass<UC3DCameraComponent>())
		{
			camComp->SetCameraMode(FGameplayTag::RequestGameplayTag("Camera.Mode.Battle"), true);
		}

		pc->Possess(leader.Pawn);
	}
}
