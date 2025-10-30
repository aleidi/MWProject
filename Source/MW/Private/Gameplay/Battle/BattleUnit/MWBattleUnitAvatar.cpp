#include "Gameplay/Battle/BattleUnit/MWBattleUnitAvatar.h"
#include "GameplayAbility/MWAbilitySet.h"

AMWBattleUnitAvatar::AMWBattleUnitAvatar(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMWBattleUnitAvatar::SetNewMesh(USkeletalMesh* NewMesh, UClass* NewAnimInst)
{
	auto* meshComp = GetMesh();

	if (!meshComp)
	{
		return;
	}

	meshComp->SetSkeletalMesh(NewMesh);

	meshComp->SetAnimInstanceClass(NewAnimInst);
}

void AMWBattleUnitAvatar::SetNewSkillTable(const FMWCharacterSkillTable& NewSkillTable)
{
	SkillTable = NewSkillTable;
}

FMWCharacterSkillTable* AMWBattleUnitAvatar::GetSkillTable()
{
	return &SkillTable;
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
