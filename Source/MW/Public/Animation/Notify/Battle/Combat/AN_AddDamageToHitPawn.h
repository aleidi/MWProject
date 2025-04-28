#pragma once

#include "Animation/Notify/MWAnimNotify.h"
#include "AN_AddDamageToHitPawn.generated.h"

UCLASS()
class UAN_AddDamageToHitPawn : public UMWAnimNotify
{
	GENERATED_BODY()

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};