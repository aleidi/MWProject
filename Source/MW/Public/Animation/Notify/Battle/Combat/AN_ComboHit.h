#pragma once

#include "Animation/Notify/MWAnimNotify.h"
#include "AN_ComboHit.generated.h"

UCLASS()
class UAN_ComboHit : public UMWAnimNotify
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "AnimNotify")
	float DamageWeight = 1.0f;

private:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};