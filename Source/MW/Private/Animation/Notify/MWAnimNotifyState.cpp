#include "Animation/Notify/MWAnimNotifyState.h"
#include "GameplayAbility/MWAbilitySystemComponent.h"

UMWAbilitySystemComponent* UMWAnimNotifyState::GetAbilitySystemComponent(USkeletalMeshComponent* MeshComp) const
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
