#include "Animation/Notify/MWAnimNotify.h"
#include "GameplayAbility/MWAbilitySystemComponent.h"

UMWAbilitySystemComponent* UMWAnimNotify::GetAbilitySystemComponent(USkeletalMeshComponent* MeshComp) const
{
	if (MeshComp)
	{
		if (AActor* owner = MeshComp->GetOwner())
		{
			return owner->GetComponentByClass<UMWAbilitySystemComponent>();
		}
	}

	return nullptr;
}
