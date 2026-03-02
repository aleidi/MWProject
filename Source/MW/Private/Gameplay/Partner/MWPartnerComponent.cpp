#include "Gameplay/Partner/MWPartnerComponent.h"

void UMWPartnerComponent::SetPartnerRole(EMWPartnerRole InPartnerRole)
{
	PartnerRole = InPartnerRole;
}

EMWPartnerRole UMWPartnerComponent::GetPartnerRole() const
{
	return PartnerRole;
}

void UMWPartnerComponent::SetPartnerActor(AActor* InPartnerActor)
{
	PartnerActor = InPartnerActor;
}

AActor* UMWPartnerComponent::GetPartnerActor() const
{
	return PartnerActor.Get();
}