#include "Animation/Notify/Battle/Combat/AN_ComboHit.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Gameplay/MWGameplayTags.h"

void UAN_ComboHit::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	AActor* ownerActor = MeshComp ? MeshComp->GetOwner() : nullptr;
	check(ownerActor);

	FGameplayEventData eventData;
	eventData.EventTag = MWGameplayTags::GP_Battle_ComboHit;
	eventData.Instigator = ownerActor;
	eventData.Target = ownerActor;
	eventData.OptionalObject = this;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(ownerActor, eventData.EventTag, eventData);
}
