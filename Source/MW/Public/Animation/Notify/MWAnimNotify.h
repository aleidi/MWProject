#pragma once

#include "Animation/AnimNotifies/AnimNotify.h"
#include "MWAnimNotify.generated.h"

UCLASS()
class UMWAnimNotify : public UAnimNotify
{
	GENERATED_BODY()

protected:
	template<typename T>
	T* GetComponent(USkeletalMeshComponent* MeshComp) const
	{
		AActor* owner = MeshComp ? MeshComp->GetOwner() : nullptr;
		if (owner)
		{
			return owner->GetComponentByClass<T>();
		}

		return nullptr;
	}
};