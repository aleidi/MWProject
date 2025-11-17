// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameplayAbility/MWAbilitySystemComponent.h"
#include "GameplayAbility/MWGameplayAbility.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MWAbilitySystemComponent)

UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_AbilityInputBlocked, "Gameplay.AbilityInputBlocked");

UMWAbilitySystemComponent::UMWAbilitySystemComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
	InputHeldSpecHandles.Reset();

	//FMemory::Memset(ActivationGroupCounts, 0, sizeof(ActivationGroupCounts));
}

void UMWAbilitySystemComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	//if (UMWGlobalAbilitySystem* GlobalAbilitySystem = UWorld::GetSubsystem<UMWGlobalAbilitySystem>(GetWorld()))
	//{
	//	GlobalAbilitySystem->UnregisterASC(this);
	//}

	Super::EndPlay(EndPlayReason);
}

void UMWAbilitySystemComponent::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
	FGameplayAbilityActorInfo* ActorInfo = AbilityActorInfo.Get();
	check(ActorInfo);
	check(InOwnerActor);

	const bool bHasNewPawnAvatar = Cast<APawn>(InAvatarActor) && (InAvatarActor != ActorInfo->AvatarActor);

	Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor);

	if (bHasNewPawnAvatar)
	{
		// Notify all abilities that a new pawn avatar has been set
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
PRAGMA_DISABLE_DEPRECATION_WARNINGS
			ensureMsgf(AbilitySpec.Ability && AbilitySpec.Ability->GetInstancingPolicy() != EGameplayAbilityInstancingPolicy::NonInstanced, TEXT("InitAbilityActorInfo: All Abilities should be Instanced (NonInstanced is being deprecated due to usability issues)."));
PRAGMA_ENABLE_DEPRECATION_WARNINGS

			TArray<UGameplayAbility*> Instances = AbilitySpec.GetAbilityInstances();
			for (UGameplayAbility* abilityInstance : Instances)
			{
				UMWGameplayAbility* wmAbilityInstance = Cast<UMWGameplayAbility>(abilityInstance);
				if (wmAbilityInstance)
				{
					// Ability instances may be missing for replays
					//abilityInstance->OnPawnAvatarSet();
				}
			}
		}

		//// Register with the global system once we actually have a pawn avatar. We wait until this time since some globally-applied effects may require an avatar.
		//if (UMWGlobalAbilitySystem* GlobalAbilitySystem = UWorld::GetSubsystem<UMWGlobalAbilitySystem>(GetWorld()))
		//{
		//	GlobalAbilitySystem->RegisterASC(this);
		//}

		//if (UMWAnimInstance* MWAnimInst = Cast<UMWAnimInstance>(ActorInfo->GetAnimInstance()))
		//{
		//	MWAnimInst->InitializeWithAbilitySystem(this);
		//}

		TryActivateAbilitiesOnSpawn();
	}
}

void UMWAbilitySystemComponent::TryActivateAbilitiesOnSpawn()
{
	ABILITYLIST_SCOPE_LOCK();

	for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
	{
		if (const UMWGameplayAbility* abilityCDO = Cast<UMWGameplayAbility>(AbilitySpec.Ability))
		{
			//abilityCDO->TryActivateAbilityOnSpawn(AbilityActorInfo.Get(), AbilitySpec);
		}
	}
}

void UMWAbilitySystemComponent::CancelAbilitiesByFunc(TShouldCancelAbilityFunc ShouldCancelFunc, bool bReplicateCancelAbility)
{
	ABILITYLIST_SCOPE_LOCK();

	for (const FGameplayAbilitySpec& abilitySpec : ActivatableAbilities.Items)
	{
		if (!abilitySpec.IsActive())
		{
			continue;
		}

		UMWGameplayAbility* abilityCDO = Cast<UMWGameplayAbility>(abilitySpec.Ability);
		if (!abilityCDO)
		{
			UE_LOG(LogMWAbilitySystem, Error, TEXT("CancelAbilitiesByFunc: Non-MWGameplayAbility %s was Granted to ASC. Skipping."), *abilitySpec.Ability.GetName());
			continue;
		}

PRAGMA_DISABLE_DEPRECATION_WARNINGS
		ensureMsgf(abilitySpec.Ability->GetInstancingPolicy() != EGameplayAbilityInstancingPolicy::NonInstanced, TEXT("CancelAbilitiesByFunc: All Abilities should be Instanced (NonInstanced is being deprecated due to usability issues)."));
PRAGMA_ENABLE_DEPRECATION_WARNINGS

		// Cancel all the spawned instances.
		TArray<UGameplayAbility*> instances = abilitySpec.GetAbilityInstances();
		for (UGameplayAbility* abilityInstance : instances)
		{
			UMWGameplayAbility* mwAbilityInstance = CastChecked<UMWGameplayAbility>(abilityInstance);

			if (ShouldCancelFunc(mwAbilityInstance, abilitySpec.Handle))
			{
				if (mwAbilityInstance->CanBeCanceled())
				{
					mwAbilityInstance->CancelAbility(abilitySpec.Handle, AbilityActorInfo.Get(), mwAbilityInstance->GetCurrentActivationInfo(), bReplicateCancelAbility);
				}
				else
				{
					UE_LOG(LogMWAbilitySystem, Error, TEXT("CancelAbilitiesByFunc: Can't cancel ability [%s] because CanBeCanceled is false."), *mwAbilityInstance->GetName());
				}
			}
		}
	}
}

void UMWAbilitySystemComponent::CancelInputActivatedAbilities(bool bReplicateCancelAbility)
{
	auto ShouldCancelFunc = [this](const UMWGameplayAbility* MWAbility, FGameplayAbilitySpecHandle Handle) -> bool
	{
		const EMWAbilityActivationPolicy ActivationPolicy = MWAbility->GetActivationPolicy();
		return ((ActivationPolicy == EMWAbilityActivationPolicy::OnInputTriggered) || (ActivationPolicy == EMWAbilityActivationPolicy::WhileInputActive));
	};

	CancelAbilitiesByFunc(ShouldCancelFunc, bReplicateCancelAbility);
}

void UMWAbilitySystemComponent::AbilitySpecInputPressed(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputPressed(Spec);

	// We don't support UGameplayAbility::bReplicateInputDirectly.
	// Use replicated events instead so that the WaitInputPress ability task works.
	if (Spec.IsActive())
	{
PRAGMA_DISABLE_DEPRECATION_WARNINGS
		const UGameplayAbility* instance = Spec.GetPrimaryInstance();
		FPredictionKey originalPredictionKey = instance ? instance->GetCurrentActivationInfo().GetActivationPredictionKey() : Spec.ActivationInfo.GetActivationPredictionKey();
PRAGMA_ENABLE_DEPRECATION_WARNINGS

		// Invoke the InputPressed event. This is not replicated here. If someone is listening, they may replicate the InputPressed event to the server.
		InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, Spec.Handle, originalPredictionKey);
	}
}

void UMWAbilitySystemComponent::AbilitySpecInputReleased(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputReleased(Spec);

	// We don't support UGameplayAbility::bReplicateInputDirectly.
	// Use replicated events instead so that the WaitInputRelease ability task works.
	if (Spec.IsActive())
	{
PRAGMA_DISABLE_DEPRECATION_WARNINGS
		const UGameplayAbility* instance = Spec.GetPrimaryInstance();
		FPredictionKey originalPredictionKey = instance ? instance->GetCurrentActivationInfo().GetActivationPredictionKey() : Spec.ActivationInfo.GetActivationPredictionKey();
PRAGMA_ENABLE_DEPRECATION_WARNINGS

		// Invoke the InputReleased event. This is not replicated here. If someone is listening, they may replicate the InputReleased event to the server.
		InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, Spec.Handle, originalPredictionKey);
	}
}

void UMWAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& abilitySpec : ActivatableAbilities.Items)
		{
			if (abilitySpec.Ability && (abilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag)))
			{
				InputPressedSpecHandles.AddUnique(abilitySpec.Handle);
				InputHeldSpecHandles.AddUnique(abilitySpec.Handle);
			}
		}
	}
}

void UMWAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& abilitySpec : ActivatableAbilities.Items)
		{
			if (abilitySpec.Ability && (abilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag)))
			{
				InputReleasedSpecHandles.AddUnique(abilitySpec.Handle);
				InputHeldSpecHandles.Remove(abilitySpec.Handle);
			}
		}
	}
}

bool UMWAbilitySystemComponent::ActorAbilityInputTagPressed(AActor* InActor, const FGameplayTag& InputTag)
{
	if (UMWAbilitySystemComponent* mwASC = InActor != nullptr ? InActor->FindComponentByClass<UMWAbilitySystemComponent>() : nullptr)
	{
		mwASC->AbilityInputTagPressed(InputTag);

		return true;
	}

	return false;
}

bool UMWAbilitySystemComponent::ActorAbilityInputTagReleased(AActor* InActor, const FGameplayTag& InputTag)
{
	if (UMWAbilitySystemComponent* mwASC = InActor != nullptr ? InActor->FindComponentByClass<UMWAbilitySystemComponent>() : nullptr)
	{
		mwASC->AbilityInputTagReleased(InputTag);

		return true;
	}

	return false;
}

void UMWAbilitySystemComponent::ProcessAbilityInput(float DeltaTime, bool bGamePaused)
{
	if (HasMatchingGameplayTag(TAG_Gameplay_AbilityInputBlocked))
	{
		ClearAbilityInput();
		return;
	}

	static TArray<FGameplayAbilitySpecHandle> AbilitiesToActivate;
	AbilitiesToActivate.Reset();

	//@TODO: See if we can use FScopedServerAbilityRPCBatcher ScopedRPCBatcher in some of these loops

	//
	// Process all abilities that activate when the input is held.
	//
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputHeldSpecHandles)
	{
		if (const FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability && !AbilitySpec->IsActive())
			{
				const UMWGameplayAbility* MWAbilityCDO = CastChecked<UMWGameplayAbility>(AbilitySpec->Ability);

				if (MWAbilityCDO->GetActivationPolicy() == EMWAbilityActivationPolicy::WhileInputActive)
				{
					AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
				}
			}
		}
	}

	//
	// Process all abilities that had their input pressed this frame.
	//
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputPressedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability)
			{
				AbilitySpec->InputPressed = true;

				if (AbilitySpec->IsActive())
				{
					// Ability is active so pass along the input event.
					AbilitySpecInputPressed(*AbilitySpec);
				}
				else
				{
					const UMWGameplayAbility* MWAbilityCDO = CastChecked<UMWGameplayAbility>(AbilitySpec->Ability);

					if (MWAbilityCDO->GetActivationPolicy() == EMWAbilityActivationPolicy::OnInputTriggered)
					{
						AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
					}
				}
			}
		}
	}

	//
	// Try to activate all the abilities that are from presses and holds.
	// We do it all at once so that held inputs don't activate the ability
	// and then also send a input event to the ability because of the press.
	//
	for (const FGameplayAbilitySpecHandle& AbilitySpecHandle : AbilitiesToActivate)
	{
		TryActivateAbility(AbilitySpecHandle);
	}

	//
	// Process all abilities that had their input released this frame.
	//
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputReleasedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability)
			{
				AbilitySpec->InputPressed = false;

				if (AbilitySpec->IsActive())
				{
					// Ability is active so pass along the input event.
					AbilitySpecInputReleased(*AbilitySpec);
				}
			}
		}
	}

	//
	// Clear the cached ability handles.
	//
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
}

void UMWAbilitySystemComponent::ClearAbilityInput()
{
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
	InputHeldSpecHandles.Reset();
}

void UMWAbilitySystemComponent::NotifyAbilityActivated(const FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability)
{
	Super::NotifyAbilityActivated(Handle, Ability);

	//if (UMWGameplayAbility* mwAbility = Cast<UMWGameplayAbility>(Ability))
	//{
	//	AddAbilityToActivationGroup(mwAbility->GetActivationGroup(), mwAbility);
	//}
}

void UMWAbilitySystemComponent::NotifyAbilityFailed(const FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability, const FGameplayTagContainer& FailureReason)
{
	Super::NotifyAbilityFailed(Handle, Ability, FailureReason);

	if (APawn* avatar = Cast<APawn>(GetAvatarActor()))
	{
		if (!avatar->IsLocallyControlled() && Ability->IsSupportedForNetworking())
		{
			//ClientNotifyAbilityFailed(Ability, FailureReason);
			return;
		}
	}

	HandleAbilityFailed(Ability, FailureReason);
}

void UMWAbilitySystemComponent::NotifyAbilityEnded(FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability, bool bWasCancelled)
{
	Super::NotifyAbilityEnded(Handle, Ability, bWasCancelled);

	//if (UMWGameplayAbility* mwAbility = Cast<UMWGameplayAbility>(Ability))
	//{
	//	RemoveAbilityFromActivationGroup(mwAbility->GetActivationGroup(), mwAbility);
	//}
}

void UMWAbilitySystemComponent::ApplyAbilityBlockAndCancelTags(const FGameplayTagContainer& AbilityTags, UGameplayAbility* RequestingAbility, bool bEnableBlockTags, const FGameplayTagContainer& BlockTags, bool bExecuteCancelTags, const FGameplayTagContainer& CancelTags)
{
	FGameplayTagContainer ModifiedBlockTags = BlockTags;
	FGameplayTagContainer ModifiedCancelTags = CancelTags;

	//if (TagRelationshipMapping)
	//{
	//	// Use the mapping to expand the ability tags into block and cancel tag
	//	//TagRelationshipMapping->GetAbilityTagsToBlockAndCancel(AbilityTags, &ModifiedBlockTags, &ModifiedCancelTags);
	//}

	Super::ApplyAbilityBlockAndCancelTags(AbilityTags, RequestingAbility, bEnableBlockTags, ModifiedBlockTags, bExecuteCancelTags, ModifiedCancelTags);

	//@TODO: Apply any special logic like blocking input or movement
}

void UMWAbilitySystemComponent::HandleChangeAbilityCanBeCanceled(const FGameplayTagContainer& AbilityTags, UGameplayAbility* RequestingAbility, bool bCanBeCanceled)
{
	Super::HandleChangeAbilityCanBeCanceled(AbilityTags, RequestingAbility, bCanBeCanceled);

	//@TODO: Apply any special logic like blocking input or movement
}

void UMWAbilitySystemComponent::GetAdditionalActivationTagRequirements(const FGameplayTagContainer& AbilityTags, FGameplayTagContainer& OutActivationRequired, FGameplayTagContainer& OutActivationBlocked) const
{
	//if (TagRelationshipMapping)
	//{
	//	//TagRelationshipMapping->GetRequiredAndBlockedActivationTags(AbilityTags, &OutActivationRequired, &OutActivationBlocked);
	//}
}

void UMWAbilitySystemComponent::SetTagRelationshipMapping(UMWAbilityTagRelationshipMapping* NewMapping)
{
	//TagRelationshipMapping = NewMapping;
}

void UMWAbilitySystemComponent::HandleAbilityFailed(const UGameplayAbility* Ability, const FGameplayTagContainer& FailureReason)
{
	//UE_LOG(LogMWAbilitySystem, Warning, TEXT("Ability %s failed to activate (tags: %s)"), *GetPathNameSafe(Ability), *FailureReason.ToString());

	if (const UMWGameplayAbility* mwAbility = Cast<const UMWGameplayAbility>(Ability))
	{
		//mwAbility->OnAbilityFailedToActivate(FailureReason);
	}	
}

void UMWAbilitySystemComponent::FindInstancedAbilityByTag(TArray<UMWGameplayAbility*>& OutAbilities, const FGameplayTagContainer& Tags)
{
	// ensure the output array is empty
	OutAbilities.Empty();

	// iterate through all Ability Specs
	for (const FGameplayAbilitySpec& currentSpec : ActivatableAbilities.Items)
	{
		// try to get the ability instance
		UGameplayAbility* abilityInstance = currentSpec.GetPrimaryInstance();

		// default to the CDO if we can't
		if (!abilityInstance)
		{
			abilityInstance = currentSpec.Ability;
		}

		// ensure the ability instance is valid
		if (IsValid(abilityInstance))
		{
			if(UMWGameplayAbility* mwAbility = Cast<UMWGameplayAbility>(abilityInstance))
			{
				// check if we match all tags
				if (mwAbility->GetAssetTags().HasAll(Tags))
				{
					// add the matching handle
					OutAbilities.Add(mwAbility);
				}
			}
		}
	}
}

void UMWAbilitySystemComponent::FindInstancedAbilityByClass(TArray<UMWGameplayAbility*>& OutAbilities, TSubclassOf<UMWGameplayAbility> InClass)
{
	// ensure the output array is empty
	OutAbilities.Empty();

	// iterate through all Ability Specs
	for (const FGameplayAbilitySpec& currentSpec : ActivatableAbilities.Items)
	{
		// try to get the ability instance
		UGameplayAbility* abilityInstance = currentSpec.GetPrimaryInstance();

		// default to the CDO if we can't
		if (!abilityInstance)
		{
			abilityInstance = currentSpec.Ability;
		}

		// ensure the ability instance is valid
		if (IsValid(abilityInstance))
		{
			if (UMWGameplayAbility* mwAbility = Cast<UMWGameplayAbility>(abilityInstance))
			{
				// check if we match all tags
				if (mwAbility->GetClass() == InClass)
				{
					// add the matching handle
					OutAbilities.Add(mwAbility);
				}
			}
		}
	}
}

bool UMWAbilitySystemComponent::ClearAllAbilitiesByActor(AActor* InActor)
{
	if (!InActor)
	{
		return false;
	}

	if (UMWAbilitySystemComponent* mwASC = InActor->FindComponentByClass<UMWAbilitySystemComponent>())
	{
		mwASC->ClearAllAbilities();
	}

	return true;
}

//bool UMWAbilitySystemComponent::IsActivationGroupBlocked(EMWAbilityActivationGroup Group) const
//{
//	bool bBlocked = false;
//
//	switch (Group)
//	{
//	case EMWAbilityActivationGroup::Independent:
//		// Independent abilities are never blocked.
//		bBlocked = false;
//		break;
//
//	case EMWAbilityActivationGroup::Exclusive_Replaceable:
//	case EMWAbilityActivationGroup::Exclusive_Blocking:
//		// Exclusive abilities can activate if nothing is blocking.
//		bBlocked = (ActivationGroupCounts[(uint8)EMWAbilityActivationGroup::Exclusive_Blocking] > 0);
//		break;
//
//	default:
//		checkf(false, TEXT("IsActivationGroupBlocked: Invalid ActivationGroup [%d]\n"), (uint8)Group);
//		break;
//	}
//
//	return bBlocked;
//}
//
//void UMWAbilitySystemComponent::AddAbilityToActivationGroup(EMWAbilityActivationGroup Group, UMWGameplayAbility* MWAbility)
//{
//	check(MWAbility);
//	check(ActivationGroupCounts[(uint8)Group] < INT32_MAX);
//
//	ActivationGroupCounts[(uint8)Group]++;
//
//	const bool bReplicateCancelAbility = false;
//
//	switch (Group)
//	{
//	case EMWAbilityActivationGroup::Independent:
//		// Independent abilities do not cancel any other abilities.
//		break;
//
//	case EMWAbilityActivationGroup::Exclusive_Replaceable:
//	case EMWAbilityActivationGroup::Exclusive_Blocking:
//		CancelActivationGroupAbilities(EMWAbilityActivationGroup::Exclusive_Replaceable, MWAbility, bReplicateCancelAbility);
//		break;
//
//	default:
//		checkf(false, TEXT("AddAbilityToActivationGroup: Invalid ActivationGroup [%d]\n"), (uint8)Group);
//		break;
//	}
//
//	const int32 ExclusiveCount = ActivationGroupCounts[(uint8)EMWAbilityActivationGroup::Exclusive_Replaceable] + ActivationGroupCounts[(uint8)EMWAbilityActivationGroup::Exclusive_Blocking];
//	if (!ensure(ExclusiveCount <= 1))
//	{
//		UE_LOG(LogMWAbilitySystem, Error, TEXT("AddAbilityToActivationGroup: Multiple exclusive abilities are running."));
//	}
//}
//
//void UMWAbilitySystemComponent::RemoveAbilityFromActivationGroup(EMWAbilityActivationGroup Group, UMWGameplayAbility* MWAbility)
//{
//	check(MWAbility);
//	check(ActivationGroupCounts[(uint8)Group] > 0);
//
//	ActivationGroupCounts[(uint8)Group]--;
//}
//
//void UMWAbilitySystemComponent::CancelActivationGroupAbilities(EMWAbilityActivationGroup Group, UMWGameplayAbility* IgnoreMWAbility, bool bReplicateCancelAbility)
//{
//	auto ShouldCancelFunc = [this, Group, IgnoreMWAbility](const UMWGameplayAbility* MWAbility, FGameplayAbilitySpecHandle Handle)
//	{
//		return ((MWAbility->GetActivationGroup() == Group) && (MWAbility != IgnoreMWAbility));
//	};
//
//	CancelAbilitiesByFunc(ShouldCancelFunc, bReplicateCancelAbility);
//}

void UMWAbilitySystemComponent::AddDynamicTagGameplayEffect(const FGameplayTag& Tag)
{
	/*const TSubclassOf<UGameplayEffect> DynamicTagGE = UMWAssetManager::GetSubclass(UMWGameData::Get().DynamicTagGameplayEffect);
	if (!DynamicTagGE)
	{
		UE_LOG(LogMWAbilitySystem, Warning, TEXT("AddDynamicTagGameplayEffect: Unable to find DynamicTagGameplayEffect [%s]."), *UMWGameData::Get().DynamicTagGameplayEffect.GetAssetName());
		return;
	}

	const FGameplayEffectSpecHandle SpecHandle = MakeOutgoingSpec(DynamicTagGE, 1.0f, MakeEffectContext());
	FGameplayEffectSpec* Spec = SpecHandle.Data.Get();

	if (!Spec)
	{
		UE_LOG(LogMWAbilitySystem, Warning, TEXT("AddDynamicTagGameplayEffect: Unable to make outgoing spec for [%s]."), *GetNameSafe(DynamicTagGE));
		return;
	}

	Spec->DynamicGrantedTags.AddTag(Tag);

	ApplyGameplayEffectSpecToSelf(*Spec);*/
}

void UMWAbilitySystemComponent::RemoveDynamicTagGameplayEffect(const FGameplayTag& Tag)
{
	/*const TSubclassOf<UGameplayEffect> DynamicTagGE = UMWAssetManager::GetSubclass(UMWGameData::Get().DynamicTagGameplayEffect);
	if (!DynamicTagGE)
	{
		UE_LOG(LogMWAbilitySystem, Warning, TEXT("RemoveDynamicTagGameplayEffect: Unable to find gameplay effect [%s]."), *UMWGameData::Get().DynamicTagGameplayEffect.GetAssetName());
		return;
	}

	FGameplayEffectQuery Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(FGameplayTagContainer(Tag));
	Query.EffectDefinition = DynamicTagGE;

	RemoveActiveEffects(Query);*/
}

void UMWAbilitySystemComponent::GetAbilityTargetData(const FGameplayAbilitySpecHandle AbilityHandle, FGameplayAbilityActivationInfo ActivationInfo, FGameplayAbilityTargetDataHandle& OutTargetDataHandle)
{
	TSharedPtr<FAbilityReplicatedDataCache> replicatedData = AbilityTargetDataMap.Find(FGameplayAbilitySpecHandleAndPredictionKey(AbilityHandle, ActivationInfo.GetActivationPredictionKey()));
	
	if (replicatedData.IsValid())
	{
		OutTargetDataHandle = replicatedData->TargetData;
	}
}

