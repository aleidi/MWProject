#include "Character/MWPawnExtensionComponent.h"
#include "GameplayAbility/MWAbilitySet.h"
#include "Character/MWPawnData.h"
#include "MWLogChannels.h"
#include "GameplayAbility/MWAbilitySystemComponent.h"
#include "Gameplay/MWGameplayTags.h"
#include "System/MWAssetManager.h"
#include "Data/MWMasterData.h"
#include "GameplayAbility/MWAbilitySet.h"
#include "Character/MWTargetSelector.h"
#include "Character/MWCharacter.h"

void UMWPawnExtensionComponent::BeginPlay()
{
    Super::BeginPlay();

    if (AMWCharacter* character = GetPawn<AMWCharacter>())
    {
        InitializeAbilitySystem(character->GetMWAbilitySystemComponent(), character);
    }
}

void UMWPawnExtensionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    TargetSelector.Reset();

    Super::EndPlay(EndPlayReason);
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

    GiveAbility();
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
        AbilityTypesToIgnore.AddTag(MWGameplayTags::Ability_Behavior_SurvivesDeath);

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

void UMWPawnExtensionComponent::GiveAbility()
{
    check(AbilitySystemComponent);

    if (AbilityGranetedHandles.IsValid())
    {
        AbilityGranetedHandles->RemoveFromAbilitySystem(AbilitySystemComponent);
    }

    if (auto data = UMWAssetManager::Get().GetMasterData())
    {
        if (data->MainCharacterData)
        {
            const FMWCharacterMasterData* char_data = data->MainCharacterData->FindRow<FMWCharacterMasterData>(PawnDataName, PawnDataName.ToString());
            if (char_data && char_data->AbilitySets.Num() > 0)
            {
                for (auto& ability_set : char_data->AbilitySets)
                {
                    ability_set->GiveToAbilitySystem(AbilitySystemComponent, AbilityGranetedHandles.Get(), GetOwner());
                }
            }
        }
    }
}

void UMWPawnExtensionComponent::InitializeTargetSelector(const AController* InControler)
{
    // set target selector
    if (!TargetSelector.IsValid())
    {
        TargetSelector = MakeUnique<FMWTargetSelector>(InControler);
    }
    else
    {
        TargetSelector->ChangeOwnerController(InControler);
    }
}

void UMWPawnExtensionComponent::SwitchToLeftTarget()
{
    if (TargetSelector.IsValid())
    {
        TargetSelector->SwitchToLeft();
    }
}

void UMWPawnExtensionComponent::SwitchToRightTarget()
{
    if (TargetSelector.IsValid())
    {
        TargetSelector->SwitchToRight();
    }
}

void UMWPawnExtensionComponent::CancelSelect()
{
    if (TargetSelector.IsValid())
    {
        TargetSelector->CancelSelect();
    }
}

void UMWPawnExtensionComponent::LockTarget()
{
    if (TargetSelector.IsValid())
    {
        TargetSelector->LockTarget();
    }
}

void UMWPawnExtensionComponent::UnlockTarget()
{
    if (TargetSelector.IsValid())
    {
        TargetSelector->UnlockTarget();
    }
}

void UMWPawnExtensionComponent::ForceLockIfNoTarget(const FMWFoundActorInfo& Target)
{
    if (TargetSelector.IsValid())
    {
        TargetSelector->ForceLockIfNoTarget(Target);
    }
}

void UMWPawnExtensionComponent::OnTargetNotExisted(const FMWFoundActorInfo& Target)
{
    if (TargetSelector.IsValid())
    {
        TargetSelector->OnTargetNotExisted(Target);
    }
}
