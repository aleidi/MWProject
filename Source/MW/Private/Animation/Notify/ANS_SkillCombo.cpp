#include "Animation/Notify/ANS_SkillCombo.h"
#include "Data/MWGameplayData.h"
#include "System/MWAssetManager.h"
#include "GameplayAbility/MWAbilitySystemComponent.h"
#include "MWLogChannels.h"

void UANS_SkillCombo::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (UMWAbilitySystemComponent* asc = GetAbilitySystemComponent(MeshComp))
	{
		TSubclassOf<UGameplayEffect> skill_combo = UMWAssetManager::Get().GetSubclass(UMWGameplayData::Get().GESkillCombo);
		if (!skill_combo)
		{
			UE_LOG(LogMWAnimNotify, Warning, TEXT("GameplayEffect class asset is not existed."));
			return;
		}
		FGameplayEffectSpecHandle spec_handle = asc->MakeOutgoingSpec(skill_combo, 1.f, asc->MakeEffectContext());
		EffectSpecHandle = asc->ApplyGameplayEffectSpecToSelf(*spec_handle.Data.Get());
	}
}

void UANS_SkillCombo::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{

}

void UANS_SkillCombo::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (EffectSpecHandle.IsValid())
	{
		if (UMWAbilitySystemComponent* asc = GetAbilitySystemComponent(MeshComp))
		{
			asc->RemoveActiveGameplayEffect(EffectSpecHandle);
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, FString::Printf(TEXT("notify end")));
		}
	}
}
