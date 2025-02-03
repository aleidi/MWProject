#pragma once

#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "MWAnimNotifyState.generated.h"

UCLASS()
class UMWAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

protected:
	class UMWAbilitySystemComponent* GetAbilitySystemComponent(USkeletalMeshComponent* MeshComp) const;
};