#include "Character/MWCharacterASC.h"
#include "GameplayAbility/MWAbilitySystemComponent.h"

AMWCharacterASC::AMWCharacterASC(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UMWAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
}

UAbilitySystemComponent* AMWCharacterASC::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UMWAbilitySystemComponent* AMWCharacterASC::GetMWAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
