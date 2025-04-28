#include "Animation/Notify/Battle/Combat/AN_AddForceToHitPawn.h"
#include "Character/MWPawnExtensionComponent.h"

void UAN_AddForceToHitPawn::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (auto* extComp = GetComponent<UMWPawnExtensionComponent>(MeshComp))
	{
		extComp->AddForce(Direction, Intensity);
	}
}
