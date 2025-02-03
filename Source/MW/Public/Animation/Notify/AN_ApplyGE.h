#pragma once

#include "Animation/Notify/MWAnimNotify.h"
#include "AN_ApplyGE.generated.h"

UCLASS()
class UAN_ApplyGE : public UMWAnimNotify
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "AnimNotify", meta = (DisplayName = "GameplayEffect Class Asset"))
	TSoftClassPtr<class UGameplayEffect> GEAsset;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};