#include "Animation/Notify/ANS_SkillPerfectCombo.h"
#include "Data/MWGameplayData.h"
#include "GameplayAbility/MWAbilitySystemComponent.h"
#include "MWGameSingleton.h"
#include "MWLogChannels.h"
#include "System/MWAssetManager.h"

void UANS_SkillPerfectCombo::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	/*if (UMWAbilitySystemComponent* asc = GetAbilitySystemComponent(MeshComp))
	{
		TSubclassOf<UGameplayEffect> perfect_combo = MWSINGLETON()->GetGameplayData()->GESkillPerfectCombo;
		if (!perfect_combo)
		{
			UE_LOG(LogMWAnimNotify, Warning, TEXT("GameplayEffect class asset is not existed."));
			return;
		}
		FGameplayEffectSpecHandle spec_handle = asc->MakeOutgoingSpec(perfect_combo, 1.f, asc->MakeEffectContext());
		EffectSpecHandle = asc->ApplyGameplayEffectSpecToSelf(*spec_handle.Data.Get());
	}*/
}

void UANS_SkillPerfectCombo::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{

}

void UANS_SkillPerfectCombo::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (EffectSpecHandle.IsValid())
	{
		if (UMWAbilitySystemComponent* asc = GetAbilitySystemComponent(MeshComp))
		{
			asc->RemoveActiveGameplayEffect(EffectSpecHandle);
		}
	}
}
