#pragma once

#include "Animation/Notify/MWAnimNotify.h"
#include "AN_KeepPawnInAir.generated.h"

UCLASS()
class UAN_KeepPawnInAir : public UMWAnimNotify
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "AnimNotify")
	bool bKeep = false;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};