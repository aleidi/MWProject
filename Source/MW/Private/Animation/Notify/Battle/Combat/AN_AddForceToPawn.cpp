#include "Animation/Notify/Battle/Combat/AN_AddForceToPawn.h"
#include "Component/Pawn/MWPawnExtensionComponent.h"

void UAN_AddForceToPawn::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (auto* extComp = GetComponent<UMWPawnExtensionComponent>(MeshComp))
	{
		extComp->AddForce(Direction, Intensity);
	}
}
