#include "MWAbilitySystemComponent.h"

bool UMWAbilitySystemComponent::TryActivateAbilityByTag(const FGameplayTag& GameplayTag, bool bAllowRemoteActivation)
{
	FGameplayAbilitySpec* AbilitiesToActivate = nullptr;
	GetActivatableGameplayAbilitySpecByMatchingTag(GameplayTag, AbilitiesToActivate);

	bool bSuccess = false;

	if (AbilitiesToActivate)
	{
		bSuccess |= TryActivateAbility(AbilitiesToActivate->Handle, bAllowRemoteActivation);
	}

	return bSuccess;
}

void UMWAbilitySystemComponent::GetActivatableGameplayAbilitySpecByMatchingTag(const FGameplayTag& GameplayTag, FGameplayAbilitySpec* MatchingGameplayAbility, bool bOnlyAbilitiesThatSatisfyTagRequirements) const
{
	if (!GameplayTag.IsValid())
	{
		return;
	}

	for (const FGameplayAbilitySpec& Spec : ActivatableAbilities.Items)
	{
		if (Spec.Ability && Spec.Ability->AbilityTags.HasTagExact(GameplayTag))
		{
			// Consider abilities that are blocked by tags currently if we're supposed to (default behavior).  
			// That way, we can use the blocking to find an appropriate ability based on tags when we have more than 
			// one ability that match the GameplayTag.
			if (!bOnlyAbilitiesThatSatisfyTagRequirements || Spec.Ability->DoesAbilitySatisfyTagRequirements(*this))
			{
				MatchingGameplayAbility = const_cast<FGameplayAbilitySpec*>(&Spec);
				return;
			}
		}
	}
}
