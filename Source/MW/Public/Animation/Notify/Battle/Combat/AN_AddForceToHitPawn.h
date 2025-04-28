#pragma once

#include "Animation/Notify/MWAnimNotify.h"
#include "AN_AddForceToHitPawn.generated.h"

UCLASS()
class UAN_AddForceToHitPawn : public UMWAnimNotify
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "AnimNotify")
	FVector Direction = FVector::UpVector;

	UPROPERTY(EditAnywhere, Category = "AnimNotify")
	float Intensity = 1.f;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};