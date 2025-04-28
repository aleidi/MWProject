#pragma once

#include "Animation/Notify/MWAnimNotify.h"
#include "AN_KeepHitPawnInAir.generated.h"

UCLASS()
class UAN_KeepHitPawnInAir : public UMWAnimNotify
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "AnimNotify")
	bool bKeep = false;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};