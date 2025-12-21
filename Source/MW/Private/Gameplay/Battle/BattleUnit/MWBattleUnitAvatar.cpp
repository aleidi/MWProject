#include "Gameplay/Battle/BattleUnit/MWBattleUnitAvatar.h"
#include "Component/Character/MWBattleUnitComponent.h"

AMWBattleUnitAvatar::AMWBattleUnitAvatar(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

	PrimaryActorTick.bCanEverTick = false;
}

void AMWBattleUnitAvatar::BeginPlay()
{
	Super::BeginPlay();
}

void AMWBattleUnitAvatar::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}
