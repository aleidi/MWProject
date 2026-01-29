#include "Gameplay/Battle/MWBattleState.h"

UMWBattleState::UMWBattleState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UMWBattleState::OnBattleStart()
{

}

void UMWBattleState::OnBattleEnd()
{

}

void UMWBattleState::AddEnemyAttendant(AActor* EnemyActor)
{

}

int32 UMWBattleState::GetCurrentEnemiesNumber() const
{
	return CurrentEnemyActors.Num();
}

float UMWBattleState::GetBattleDuration() const
{
	return 0.f;
}
