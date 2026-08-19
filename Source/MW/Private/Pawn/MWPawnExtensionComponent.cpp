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
        // 旧Ability System Componentをクリーンアップ
        UninitializeAbilitySystem();
    }

    APawn* pawn = GetPawnChecked<APawn>();
    AActor* exist_avatar = InASC->GetAvatarActor();

    if (exist_avatar != nullptr && exist_avatar != pawn)
    {
        // ASCのAvatarとして設定済みのPawnを解除
        // クライアント遅延時、死亡Pawnの削除前に新Pawnが生成・Possessされると発生し得る
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

    // 自身がAvatar ActorのままならASCを初期化解除（別PawnがAvatarなら処理済み）
    if (AbilitySystemComponent->GetAvatarActor() == GetOwner())
    {
        FGameplayTagContainer AbilityTypesToIgnore;
        AbilitySystemComponent->CancelAbilities(nullptr, &AbilityTypesToIgnore);
        AbilitySystemComponent->ClearAllAbilities();
        AbilitySystemComponent->ClearAbilityInput();
        // Cueを削除
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