#include "CombatRelationLibrary.h"

UCombatRelationComponent* UCombatRelationLibrary::FindRelationComponent(AActor* Other)
{
	return Other ? Other->FindComponentByClass<UCombatRelationComponent>() : nullptr;
}

const UCombatRelationComponent* UCombatRelationLibrary::FindRelationComponent(const AActor* Other)
{
	return Other ? Other->FindComponentByClass<UCombatRelationComponent>() : nullptr;
}

ETeamAttitude::Type UCombatRelationLibrary::GetTeamAttitudeTowards(const AActor* Other)
{
	const IGenericTeamAgentInterface* teamAgent = Cast<const IGenericTeamAgentInterface>(Other);
	if (!teamAgent)
	{
		return ETeamAttitude::Neutral;
	}

	if (const UCombatRelationComponent* CRComp = FindRelationComponent(Other))
	{
		switch (CRComp->GetRelationTo(Other))
		{
			case ECombatRelation::Friendly:
				return ETeamAttitude::Friendly;

			case ECombatRelation::Hostile:
				return ETeamAttitude::Hostile;

			default:
				return ETeamAttitude::Neutral;
		}
	}

	return ETeamAttitude::Neutral;
}
