#include "Pawn/MWPawnExtensionComponent.h"

#include "Character/MWCharacter.h"
#include "Character/MWTargetSelector.h"
#include "Data/MWMasterData.h"
#include "Define/MWDefineDataTable.h"
#include "Gameplay/MWGameplayTags.h"
#include "GameplayAbility/MWAbilitySet.h"
#include "GameplayAbility/MWAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MWGameSingleton.h"
#include "MWLogChannels.h"

void UMWPawnExtensionComponent::BeginPlay()
{
    Super::BeginPlay();

    if (AMWCharacter* character = GetPawn<AMWCharacter>())
    {
        // Initialize character data

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

    const FMWCharacterTemplateData* characterData = /*DATATABLEMANAGER()->GetCharacterDataById(CharacterId)*/nullptr;
	if (characterData && characterData->BaseAbilitySets.Num() > 0)
	{
		for (const TSoftObjectPtr<UMWAbilitySet>& ability_set_ptr : characterData->BaseAbilitySets)
		{
			// Check if the ability set is loaded
			UMWAbilitySet* ability_set = ability_set_ptr.Get();
			if (!ability_set)
			{
				// Synchronously load the ability set if not loaded
				ability_set = ability_set_ptr.LoadSynchronous();
			}

			// Verify the ability set was loaded successfully before using it
			if (ability_set)
			{
				ability_set->GiveToAbilitySystem(AbilitySystemComponent, AbilityGranetedHandles.Get(), GetOwner());
			}
			else
			{
				UE_LOG(LogMWComponent, Warning, TEXT("Failed to load ability set for character %d in %s"), CharacterId, *GetNameSafe(GetOwner()));
			}
		}
	}
}

void UMWPawnExtensionComponent::InitializeTargetSelector(const AController* InControler)
{
    // set target selector
    if (!TargetSelector.IsValid())
    {
        TargetSelector = MakeShared<FMWTargetSelector>(InControler);
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

void UMWPawnExtensionComponent::CastSkill(const FGameplayTag& SkillTag, FGameplayEventData Payload)
{
    if (AbilitySystemComponent.Get())
    {
        AbilitySystemComponent->HandleGameplayEvent(SkillTag, &Payload);
    }
}

void UMWPawnExtensionComponent::AddForce(const FVector& Dir, float Intensity)
{
    if (auto* pawn = GetPawnChecked<APawn>())
    {
        if (auto* moveComp = pawn->GetComponentByClass<UCharacterMovementComponent>())
        {
            moveComp->AddImpulse(Dir * Intensity * 1000.f, true);
        }
    }
}

void UMWPawnExtensionComponent::AffectedByGravity(bool bAffect)
{
    if (auto* pawn = GetPawnChecked<APawn>())
    {
        if (auto* moveComp = pawn->GetComponentByClass<UCharacterMovementComponent>())
        {
            moveComp->SetMovementMode(bAffect ? EMovementMode::MOVE_Walking : EMovementMode::MOVE_None);
        }
    }
}
