#include "Animation/Notify/Battle/Combat/AN_AddForceToHitPawn.h"
#include "Component/Pawn/MWPawnExtensionComponent.h"

void UAN_AddForceToHitPawn::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (auto* extComp = GetComponent<UMWPawnExtensionComponent>(MeshComp))
	{
		extComp->AddForce(Direction, Intensity);
	}
}
