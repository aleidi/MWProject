#include "Animation/Notify/AN_ApplyGE.h"
#include "GameplayAbility/MWAbilitySystemComponent.h"
#include "System/MWAssetManager.h"
#include "GameplayEffect.h"

void UAN_ApplyGE::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (AActor* owner = MeshComp->GetOwner())
	{
		if (UMWAbilitySystemComponent* asc = owner->GetComponentByClass<UMWAbilitySystemComponent>())
		{
			TSubclassOf<UGameplayEffect> asset_class = UMWAssetManager::Get().GetSubclass(GEAsset);
			FGameplayEffectSpecHandle spec_handle = asc->MakeOutgoingSpec(asset_class, 1.f, asc->MakeEffectContext());
			asc->ApplyGameplayEffectSpecToSelf(*spec_handle.Data.Get());
		}
	}
}
