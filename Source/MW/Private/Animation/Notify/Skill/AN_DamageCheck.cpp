#include "Animation/Notify/Skill/AN_DamageCheck.h"

#include "AbilitySystemGlobals.h"
#include "GameplayAbility/Ability/MWChargeableSkill.h"
#include "GameplayAbility/MWAbilitySystemComponent.h"

void UAN_DamageCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp || !MeshComp->GetOwner())
	{
		return;
	}

	AActor* ownerActor = MeshComp->GetOwner();

	// Get Ability System Component from the owner actor
	UAbilitySystemComponent* asc = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(ownerActor);
	if (!asc)
	{
		UE_LOG(LogMWAnimNotify, Warning, TEXT("[AN_DamageCheck] Owner actor does not have AbilitySystemComponent"));
		return;
	}

	// Find active MWChargeableSkill ability instance
	TArray<FGameplayAbilitySpec>& activatableAbilities = asc->GetActivatableAbilities();
	for (FGameplayAbilitySpec& spec : activatableAbilities)
	{
		if (spec.IsActive())
		{
			// Try to cast to MWChargeableSkill
			UMWChargeableSkill* chargeableSkill = Cast<UMWChargeableSkill>(spec.GetPrimaryInstance());
			if (chargeableSkill)
			{
				// Trigger damage calculation with configured parameters
				chargeableSkill->OnDamageNotify(DamageEventTag, DamageMultiplier, HitIndex);
				
				// Found and triggered the skill, no need to continue
				return;
			}
		}
	}

	// Log warning if no active chargeable skill was found
	UE_LOG(LogMWAnimNotify, Warning, TEXT("[AN_DamageCheck] No active MWChargeableSkill found on actor: %s"), *ownerActor->GetName());
}

FString UAN_DamageCheck::GetNotifyName_Implementation() const
{
	if (DamageEventTag.IsValid())
	{
		return FString::Printf(TEXT("Damage Check [%s] x%.2f (Hit %d)"), 
			*DamageEventTag.ToString(), 
			DamageMultiplier, 
			HitIndex);
	}
	
	return FString::Printf(TEXT("Damage Check x%.2f (Hit %d)"), DamageMultiplier, HitIndex);
}