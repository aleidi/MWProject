#pragma once

#include "Animation/AnimNotifies/AnimNotify.h"
#include "MWAnimNotify.generated.h"

UCLASS()
class UMWAnimNotify : public UAnimNotify
{
	GENERATED_BODY()

protected:
	class UMWAbilitySystemComponent* GetAbilitySystemComponent(USkeletalMeshComponent* MeshComp) const;
};