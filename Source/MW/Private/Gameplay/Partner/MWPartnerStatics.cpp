#include "Gameplay/Partner/MWPartnerStatics.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Gameplay/Partner/MWPartnerComponent.h"

AActor* UMWPartnerStatics::GetPartnerActor(AActor* Outer)
{
	if (!Outer)
	{
		return nullptr;
	}

	UMWPartnerComponent* partnerComp = Outer->FindComponentByClass<UMWPartnerComponent>();
	if (!partnerComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("GetPartnerActor: No MWPartnerComponent found on %s"), *Outer->GetName());
	}

	AActor* partnerActor = partnerComp ? partnerComp->GetPartnerActor() : nullptr;

	if (!partnerActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("GetPartnerActor: No partner actor found in MWPartnerComponent on %s"), *Outer->GetName());
	}

	return partnerActor;
}

void UMWPartnerStatics::SendGameplayEventToPartner(AActor* Outer, FGameplayTag EventTag, FGameplayEventData Payload)
{
	if (!Outer)
	{
		return;
	}

	AActor* partnerActor = GetPartnerActor(Outer);

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(partnerActor, EventTag, Payload);
}
