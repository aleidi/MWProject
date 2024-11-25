#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "MWAbilitySystemComponent.generated.h"

UCLASS()
class MW_API UMWAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	bool TryActivateAbilityByTag(const FGameplayTag& GameplayTag, bool bAllowRemoteActivation = true);

private:
	void GetActivatableGameplayAbilitySpecByMatchingTag(const FGameplayTag& GameplayTag, struct FGameplayAbilitySpec* MatchingGameplayAbility, bool bOnlyAbilitiesThatSatisfyTagRequirements = true) const;

};