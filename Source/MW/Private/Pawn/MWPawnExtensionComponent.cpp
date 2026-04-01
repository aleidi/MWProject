#include "Pawn/MWPawnExtensionComponent.h"

#include "GameplayAbility/MWAbilitySet.h"
#include "GameplayAbility/MWAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MWGameSingleton.h"
#include "MWLogChannels.h"

void UMWPawnExtensionComponent::BeginPlay()
{
    Super::BeginPlay();

	AActor* owner = GetOwner();
	UMWAbilitySystemComponent* asc = owner ? owner->FindComponentByClass<UMWAbilitySystemComponent>() : nullptr;
    InitializeAbilitySystem(asc, owner);
}

void UMWPawnExtensionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    UninitializeAbilitySystem();
}

UMWPawnExtensionComponent* UMWPawnExtensionComponent::FindPawnExtensionComponent(const AActor* Actor)
{
    return Actor ? Actor->FindComponentByClass<UMWPawnExtensionComponent>() : nullptr;
}

void UMWPawnExtensionComponent::InitializeAbilitySystem(UMWAbilitySystemComponent* InASC, AActor* OwnerActor)
{
    ensure(InASC);
    ensure(OwnerActor);

    if (InASC == AbilitySystemComponent)
    {
        UE_LOG(LogMWComponent, Warning, TEXT("%s has already initialize AbilitySystemComponent"), *GetNameSafe(OwnerActor));
        return;
    }

    if (AbilitySystemComponent)
    {
        // Clean up the old ability system component.
        UninitializeAbilitySystem();
    }

    APawn* pawn = GetPawnChecked<APawn>();
    AActor* exist_avatar = InASC->GetAvatarActor();

    if (exist_avatar != nullptr && exist_avatar != pawn)
    {
        // There is already a pawn acting as the ASC's avatar, so we need to kick it out
        // This can happen on clients if they're lagged: their new pawn is spawned + possessed before the dead one is removed
        if (UMWPawnExtensionComponent* OtherExtensionComponent = FindPawnExtensionComponent(exist_avatar))
        {
            OtherExtensionComponent->UninitializeAbilitySystem();
        }
    }

    AbilitySystemComponent = InASC;
    AbilitySystemComponent->InitAbilityActorInfo(OwnerActor, pawn);

    if (OnAbilitySystemInitialized.IsBound())
    {
        OnAbilitySystemInitialized.Broadcast();
    }
}

void UMWPawnExtensionComponent::UninitializeAbilitySystem()
{
    if (!AbilitySystemComponent)
    {
        return;
    }

    // Uninitialize the ASC if we're still the avatar actor (otherwise another pawn already did it when they became the avatar actor)
    if (AbilitySystemComponent->GetAvatarActor() == GetOwner())
    {
        FGameplayTagContainer AbilityTypesToIgnore;
        AbilitySystemComponent->CancelAbilities(nullptr, &AbilityTypesToIgnore);
        AbilitySystemComponent->ClearAllAbilities();
        AbilitySystemComponent->ClearAbilityInput();
        // remove cue
        AbilitySystemComponent->RemoveAllGameplayCues();

        if (AbilitySystemComponent->GetOwnerActor() != nullptr)
        {
            AbilitySystemComponent->SetAvatarActor(nullptr);
        }
        else
        {
            // If the ASC doesn't have a valid owner, we need to clear all actor info, not just the avatar pairing
            AbilitySystemComponent->ClearActorInfo();
        }

        
        if (OnAbilitySystemUninitialized.IsBound())
        {
            OnAbilitySystemUninitialized.Broadcast();
        }
    }

    AbilitySystemComponent = nullptr;
}

int32 UMWPawnExtensionComponent::GetPawnDataId() const
{
    return PawnDataId;
}

void UMWPawnExtensionComponent::SetPawnDataId(int32 NewId)
{
    PawnDataId = NewId;
}
