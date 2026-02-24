#include "GameFeatures/GameFeatureAction_AddAbilities.h"

#include "Engine/GameInstance.h"
#include "Components/GameFrameworkComponentManager.h"
#include "GameplayAbility/MWAbilitySystemComponent.h"
#include "Engine/World.h"
//#include "Player/MWPlayerState.h" //@TODO: For the fname
#include "GameFeatures/GameFeatureAction_WorldActionBase.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

#include UE_INLINE_GENERATED_CPP_BY_NAME(GameFeatureAction_AddAbilities)

#define LOCTEXT_NAMESPACE "GameFeatures"

//////////////////////////////////////////////////////////////////////
// UGameFeatureAction_AddAbilities

void UGameFeatureAction_AddAbilities::OnGameFeatureActivating(FGameFeatureActivatingContext& Context)
{
	FPerContextData& activeData = ContextData.FindOrAdd(Context);

	if (!ensureAlways(activeData.ActiveExtensions.IsEmpty()) ||
		!ensureAlways(activeData.ComponentRequests.IsEmpty()))
	{
		Reset(activeData);
	}
	Super::OnGameFeatureActivating(Context);
}

void UGameFeatureAction_AddAbilities::OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context)
{
	Super::OnGameFeatureDeactivating(Context);
	FPerContextData* activeData = ContextData.Find(Context);

	if (ensure(activeData))
	{
		Reset(*activeData);
	}
}

#if WITH_EDITOR
EDataValidationResult UGameFeatureAction_AddAbilities::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult result = CombineDataValidationResults(Super::IsDataValid(Context), EDataValidationResult::Valid);

	int32 entryIndex = 0;
	for (const FGameFeatureAbilitiesEntry& entry : AbilitiesList)
	{
		if (entry.ActorClass.IsNull())
		{
			result = EDataValidationResult::Invalid;
			Context.AddError(FText::Format(LOCTEXT("EntryHasNullActor", "Null ActorClass at index {0} in AbilitiesList"), FText::AsNumber(entryIndex)));
		}

		if (entry.GrantedAbilities.IsEmpty() && entry.GrantedAttributes.IsEmpty() && entry.GrantedAbilitySets.IsEmpty())
		{
			result = EDataValidationResult::Invalid;
			Context.AddError(FText::Format(LOCTEXT("EntryHasNoAddOns", "Index {0} in AbilitiesList will do nothing (no granted abilities, attributes, or ability sets)"), FText::AsNumber(entryIndex)));
		}

		int32 abilityIndex = 0;
		for (const FMWAbilityGrant& ability : entry.GrantedAbilities)
		{
			if (ability.AbilityType.IsNull())
			{
				result = EDataValidationResult::Invalid;
				Context.AddError(FText::Format(LOCTEXT("EntryHasNullAbility", "Null AbilityType at index {0} in AbilitiesList[{1}].GrantedAbilities"), FText::AsNumber(abilityIndex), FText::AsNumber(entryIndex)));
			}
			++abilityIndex;
		}

		int32 attributesIndex = 0;
		for (const FMWAttributeSetGrant& attributes : entry.GrantedAttributes)
		{
			if (attributes.AttributeSetType.IsNull())
			{
				result = EDataValidationResult::Invalid;
				Context.AddError(FText::Format(LOCTEXT("EntryHasNullAttributeSet", "Null AttributeSetType at index {0} in AbilitiesList[{1}].GrantedAttributes"), FText::AsNumber(attributesIndex), FText::AsNumber(entryIndex)));
			}
			++attributesIndex;
		}

		int32 attributeSetIndex = 0;
		for (const TSoftObjectPtr<const UMWAbilitySet>& attributeSetPtr : entry.GrantedAbilitySets)
		{
			if (attributeSetPtr.IsNull())
			{
				result = EDataValidationResult::Invalid;
				Context.AddError(FText::Format(LOCTEXT("EntryHasNullAttributeSet", "Null AbilitySet at index {0} in AbilitiesList[{1}].GrantedAbilitySets"), FText::AsNumber(attributeSetIndex), FText::AsNumber(entryIndex)));
			}
			++attributeSetIndex;
		}
		++entryIndex;
	}

	return result;

	return EDataValidationResult::NotValidated;
}
#endif

void UGameFeatureAction_AddAbilities::AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext)
{
	UWorld* world = WorldContext.World();
	UGameInstance* gameInstance = WorldContext.OwningGameInstance;
	FPerContextData& activeData = ContextData.FindOrAdd(ChangeContext);

	if ((gameInstance != nullptr) && (world != nullptr) && world->IsGameWorld())
	{
		if (UGameFrameworkComponentManager* componentMan = UGameInstance::GetSubsystem<UGameFrameworkComponentManager>(gameInstance))
		{
			int32 entryIndex = 0;
			for (const FGameFeatureAbilitiesEntry& entry : AbilitiesList)
			{
				if (!entry.ActorClass.IsNull())
				{
					UGameFrameworkComponentManager::FExtensionHandlerDelegate addAbilitiesDelegate = UGameFrameworkComponentManager::FExtensionHandlerDelegate::CreateUObject(
						this, &UGameFeatureAction_AddAbilities::HandleActorExtension, entryIndex, ChangeContext);
					TSharedPtr<FComponentRequestHandle> extensionRequestHandle = componentMan->AddExtensionHandler(entry.ActorClass, addAbilitiesDelegate);

					activeData.ComponentRequests.Add(extensionRequestHandle);
					entryIndex++;
				}
			}
		}
	}
}

void UGameFeatureAction_AddAbilities::Reset(FPerContextData& ActiveData)
{
	while (!ActiveData.ActiveExtensions.IsEmpty())
	{
		auto extensionIt = ActiveData.ActiveExtensions.CreateIterator();
		RemoveActorAbilities(extensionIt->Key, ActiveData);
	}

	ActiveData.ComponentRequests.Empty();
}

void UGameFeatureAction_AddAbilities::HandleActorExtension(AActor* Actor, FName EventName, int32 EntryIndex, FGameFeatureStateChangeContext ChangeContext)
{
	FPerContextData* activeData = ContextData.Find(ChangeContext);
	if (AbilitiesList.IsValidIndex(EntryIndex) && activeData)
	{
		const FGameFeatureAbilitiesEntry& entry = AbilitiesList[EntryIndex];
		if ((EventName == UGameFrameworkComponentManager::NAME_ExtensionRemoved) || (EventName == UGameFrameworkComponentManager::NAME_ReceiverRemoved))
		{
			RemoveActorAbilities(Actor, *activeData);
		}
		else if ((EventName == UGameFrameworkComponentManager::NAME_ExtensionAdded) /*|| (EventName == AMWPlayerState::NAME_MWAbilityReady)*/)
		{
			AddActorAbilities(Actor, entry, *activeData);
		}
	}
}

void UGameFeatureAction_AddAbilities::AddActorAbilities(AActor* Actor, const FGameFeatureAbilitiesEntry& AbilitiesEntry, FPerContextData& ActiveData)
{
	check(Actor);
	if (!Actor->HasAuthority())
	{
		return;
	}

	// early out if Actor already has ability extensions applied
	if (ActiveData.ActiveExtensions.Find(Actor) != nullptr)
	{
		return;	
	}

	if (UAbilitySystemComponent* abilitySystemComponent = FindOrAddComponentForActor<UAbilitySystemComponent>(Actor, AbilitiesEntry, ActiveData))
	{
		FActorExtensions addedExtensions;
		addedExtensions.Abilities.Reserve(AbilitiesEntry.GrantedAbilities.Num());
		addedExtensions.Attributes.Reserve(AbilitiesEntry.GrantedAttributes.Num());
		addedExtensions.AbilitySetHandles.Reserve(AbilitiesEntry.GrantedAbilitySets.Num());

		for (const FMWAbilityGrant& ability : AbilitiesEntry.GrantedAbilities)
		{
			if (!ability.AbilityType.IsNull())
			{
				FGameplayAbilitySpec newAbilitySpec(ability.AbilityType.LoadSynchronous());
				FGameplayAbilitySpecHandle abilityHandle = abilitySystemComponent->GiveAbility(newAbilitySpec);

				addedExtensions.Abilities.Add(abilityHandle);
			}
		}

		for (const FMWAttributeSetGrant& attributes : AbilitiesEntry.GrantedAttributes)
		{
			if (!attributes.AttributeSetType.IsNull())
			{
				TSubclassOf<UAttributeSet> setType = attributes.AttributeSetType.LoadSynchronous();
				if (setType)
				{
					UAttributeSet* newSet = NewObject<UAttributeSet>(abilitySystemComponent->GetOwner(), setType);
					if (!attributes.InitializationData.IsNull())
					{
						UDataTable* initData = attributes.InitializationData.LoadSynchronous();
						if (initData)
						{
							newSet->InitFromMetaDataTable(initData);
						}
					}

					addedExtensions.Attributes.Add(newSet);
					abilitySystemComponent->AddAttributeSetSubobject(newSet);
				}
			}
		}

		UMWAbilitySystemComponent* mwasc = CastChecked<UMWAbilitySystemComponent>(abilitySystemComponent);
		if (!mwasc)
		{
			return;
		}

		for (const TSoftObjectPtr<const UMWAbilitySet>& setPtr : AbilitiesEntry.GrantedAbilitySets)
		{
			if (const UMWAbilitySet* set = setPtr.Get())
			{
				set->GiveToAbilitySystem(mwasc, &addedExtensions.AbilitySetHandles.AddDefaulted_GetRef());
			}
		}

		ActiveData.ActiveExtensions.Add(Actor, addedExtensions);
	}
	else
	{
		UE_LOG(LogGameFeatures, Error, TEXT("Failed to find/add an ability component to '%s'. Abilities will not be granted."), *Actor->GetPathName());
	}
}

void UGameFeatureAction_AddAbilities::RemoveActorAbilities(AActor* Actor, FPerContextData& ActiveData)
{
	if (FActorExtensions* actorExtensions = ActiveData.ActiveExtensions.Find(Actor))
	{
		if (UAbilitySystemComponent* abilitySystemComponent = Actor->FindComponentByClass<UAbilitySystemComponent>())
		{
			for (UAttributeSet* attribSetInstance : actorExtensions->Attributes)
			{
				abilitySystemComponent->RemoveSpawnedAttribute(attribSetInstance);
			}

			for (FGameplayAbilitySpecHandle abilityHandle : actorExtensions->Abilities)
			{
				abilitySystemComponent->SetRemoveAbilityOnEnd(abilityHandle);
			}

			UMWAbilitySystemComponent* mwasc = CastChecked<UMWAbilitySystemComponent>(abilitySystemComponent);
			for (FMWAbilitySet_GrantedHandles& setHandle : actorExtensions->AbilitySetHandles)
			{
				setHandle.RemoveFromAbilitySystem(mwasc);
			}
		}

		ActiveData.ActiveExtensions.Remove(Actor);
	}
}

UActorComponent* UGameFeatureAction_AddAbilities::FindOrAddComponentForActor(UClass* ComponentType, AActor* Actor, const FGameFeatureAbilitiesEntry& AbilitiesEntry, FPerContextData& ActiveData)
{
	UActorComponent* component = Actor->FindComponentByClass(ComponentType);
	
	bool bMakeComponentRequest = (component == nullptr);
	if (component)
	{
		// Check to see if this component was created from a different `UGameFrameworkComponentManager` request.
		// `Native` is what `CreationMethod` defaults to for dynamically added components.
		if (component->CreationMethod == EComponentCreationMethod::Native)
		{
			// Attempt to tell the difference between a true native component and one created by the GameFrameworkComponent system.
			// If it is from the UGameFrameworkComponentManager, then we need to make another request (requests are ref counted).
			UObject* componentArchetype = component->GetArchetype();
			bMakeComponentRequest = componentArchetype->HasAnyFlags(RF_ClassDefaultObject);
		}
	}

	if (bMakeComponentRequest)
	{
		UWorld* world = Actor->GetWorld();
		UGameInstance* gameInstance = world->GetGameInstance();

		if (UGameFrameworkComponentManager* componentMan = UGameInstance::GetSubsystem<UGameFrameworkComponentManager>(gameInstance))
		{
			TSharedPtr<FComponentRequestHandle> requestHandle = componentMan->AddComponentRequest(AbilitiesEntry.ActorClass, ComponentType);
			ActiveData.ComponentRequests.Add(requestHandle);
		}

		if (!component)
		{
			component = Actor->FindComponentByClass(ComponentType);
			ensureAlways(component);
		}
	}

	return component;
}

#undef LOCTEXT_NAMESPACE

