#pragma once

#include "Animation/Notify/MWAnimNotifyState.h"
#include "ActiveGameplayEffectHandle.h"
#include "ANS_SkillPerfectCombo.generated.h"

UCLASS()
class UANS_SkillPerfectCombo : public UMWAnimNotifyState
{
	GENERATED_BODY()

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	UPROPERTY()
	FActiveGameplayEffectHandle EffectSpecHandle;
};