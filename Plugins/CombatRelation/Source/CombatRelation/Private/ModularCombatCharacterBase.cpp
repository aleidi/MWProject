#include "ModularCombatCharacterBase.h"

#include "CombatRelationComponent.h"

AModularCombatCharacterBase::AModularCombatCharacterBase(const FObjectInitializer& Obj)
	: Super(Obj)
{
}

FGenericTeamId AModularCombatCharacterBase::GetGenericTeamId() const
{
	if (const UCombatRelationComponent* CRComp = UCombatRelationLibrary::FindRelationComponent(this))
	{
		return FGenericTeamId(CRComp->TeamId);
	}

	return FGenericTeamId::NoTeam;
}

void AModularCombatCharacterBase::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	if (UCombatRelationComponent* CRComp = UCombatRelationLibrary::FindRelationComponent(this))
	{
		CRComp->TeamId = NewTeamID.GetId();
	}
}

ETeamAttitude::Type AModularCombatCharacterBase::GetTeamAttitudeTowards(const AActor& Other) const
{
	return UCombatRelationLibrary::GetTeamAttitudeTowards(&Other);
}
