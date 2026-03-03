// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnhancedPlayerInput.h"
#include "GameplayTagContainer.h"
#include "MWInputConfig.generated.h"


class UInputAction;
class UInputMappingContext;
class UMWLocalPlayer;

/**
 * FMWInputAction
 *
 *	Struct used to map a input action to a gameplay input tag.
 */
USTRUCT(BlueprintType)
struct FMWInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<const UInputAction> InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag;

	UPROPERTY(EditDefaultsOnly)
	ETriggerEvent PressedTriggerEvent = ETriggerEvent::Started;

	UPROPERTY(EditDefaultsOnly)
	ETriggerEvent ReleasedTriggerEvent = ETriggerEvent::Completed;
};

USTRUCT(BlueprintType)
struct FMWInputMappingContextWithPriority
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputMappingContext> Mapping;

	/* High priority mapping would override low priority. */
	UPROPERTY(EditDefaultsOnly)
	int32 Priority = 0;

	bool operator==(const FMWInputMappingContextWithPriority& Other) const
	{
		return Mapping == Other.Mapping;
	}
};

USTRUCT(BlueprintType)
struct FMWInputActionContainer
{
	GENERATED_BODY()

	// List of input actions used by the owner.  These input actions are mapped to a gameplay tag and must be manually bound.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "InputAction"))
	TArray<FMWInputAction> NativeInputActions;

	// List of input actions used by the owner.  These input actions are mapped to a gameplay tag and are automatically bound to abilities with matching input tags.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "InputAction"))
	TArray<FMWInputAction> AbilityInputActions;
};

/**
 * UMWInputConfig
 *
 *	Non-mutable data asset that contains input configuration properties.
 */
UCLASS(BlueprintType, Const)
class UMWInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	const UInputAction* FindNativeInputActionForTag(const FGameplayTag& IMCTag, const FGameplayTag& InputActionTag) const;
	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& IMCTag, const FGameplayTag& InputActionTag) const;
	bool GetAbilityInputActionsForTag(const FGameplayTag& IMCTag, TArray<FMWInputAction>& OutActions) const;

public:
	/* Key is the input mapping context that owns the input actions. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FGameplayTag, FMWInputActionContainer> InputActionsContainers;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "Mapping"))
	TMap<FGameplayTag, FMWInputMappingContextWithPriority> InputMappingContext;
};
