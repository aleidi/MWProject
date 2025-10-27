#include "Animation/Notify/Battle/Combat/AN_KeepPawnInAir.h"
#include "Component/Pawn/MWPawnExtensionComponent.h"

void UAN_KeepPawnInAir::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (auto* extComp = GetComponent<UMWPawnExtensionComponent>(MeshComp))
	{
		extComp->AffectedByGravity(!bKeep);
	}
}
