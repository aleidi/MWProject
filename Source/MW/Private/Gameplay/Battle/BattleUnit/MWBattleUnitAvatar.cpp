#include "Gameplay/Battle/BattleUnit/MWBattleUnitAvatar.h"
#include "Component/Character/MWBattleUnitComponent.h"
#include "GameplayAbility/MWAbilitySet.h"

AMWBattleUnitAvatar::AMWBattleUnitAvatar(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	BattleUnitComponent = CreateDefaultSubobject<UMWBattleUnitComponent>(TEXT("BattleUnitComponent"));

	PrimaryActorTick.bCanEverTick = false;
}

void AMWBattleUnitAvatar::InitAbility()
{
	if (AbilitySet)
	{
		AbilitySet->GiveToAbilitySystem(GetMWAbilitySystemComponent(), AbilityGranetedHandles.Get(), this);
	}
}

void AMWBattleUnitAvatar::BeginPlay()
{
	Super::BeginPlay();

	InitAbility();
}

void AMWBattleUnitAvatar::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}
