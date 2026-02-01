#include "Animation/Notify/Skill/AN_InterruptibleWindowCheck.h"

#include "GameplayAbility/MWAbilitySystemComponent.h"
#include "Gameplay/MWGameplayTags.h"
#include "Gameplay/MWGameplayUtility.h"

void UAN_InterruptibleWindowCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp)
	{
		return;
	}

	AActor* owner = MeshComp->GetOwner();
	if(!owner)
	{
		UE_LOG(LogTemp, Warning, TEXT("[AN_InterruptibleWindowCheck] MeshComp has no owner."));

		return;
	}

	UMWAbilitySystemComponent* mwASC = owner->GetComponentByClass<UMWAbilitySystemComponent>();
	if (!mwASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("[AN_InterruptibleWindowCheck] Owner has no MWAbilitySystemComponent."));

		return;
	}

	// Get the gameplay tag corresponding to the charge stage to check
	for (EMWInputChargeStage inputChargeStage : InputChargeStageToCheck)
	{
		FGameplayTag chargeStageTag = UWMGameplayUtility::GetInputChargeStageTag(inputChargeStage);
		if(!chargeStageTag.IsValid())
		{
			break;
		}

		// Check if the ability system component has the matching charge stage tag
		if (mwASC->HasMatchingGameplayTag(chargeStageTag))
		{
			// Remove the charge stage tag as it's no longer needed
			mwASC->RemoveLooseGameplayTag(chargeStageTag);

			// Remove the uninterruptible tag to allow next action
			if (mwASC->HasMatchingGameplayTag(MWGameplayTags::Ability_ActionUninterruptible))
			{
				mwASC->RemoveLooseGameplayTag(MWGameplayTags::Ability_ActionUninterruptible);
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, FString::Printf(TEXT("[AN_ComboWindowCheck] Removed Ability_ActionUninterruptible on [%s]"), *owner->GetName()));
				UE_LOG(LogTemp, Log, TEXT("[AN_ComboWindowCheck] Combo window opened for charge stage: %s on [%s]"),
					*chargeStageTag.ToString(),
					*owner->GetName());

				return;
			}
		}
		else
		{
			UE_LOG(LogTemp, Verbose, TEXT("[AN_ComboWindowCheck] Charge stage tag [%s] not found on [%s], combo window not opened."),
				*chargeStageTag.ToString(),
				*owner->GetName());
		}

	}
}

