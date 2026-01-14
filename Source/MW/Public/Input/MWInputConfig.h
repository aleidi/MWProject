// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "InputTriggers.h"
#include "MWInputConfig.generated.h"

class UInputAction;
class UInputMappingContext;
class UMWLocalPlayer;

USTRUCT(BlueprintType)
struct FMWInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<const UInputAction> InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag;

	// Used for ability input actions to define which trigger events correspond to press and release.
	// 入力アクションが押されたときと離されたときに対応するトリガーイベントを定義するために使用される。
	UPROPERTY(EditDefaultsOnly)
	ETriggerEvent PressEventType = ETriggerEvent::Started;

	// Used for ability input actions to define which trigger events correspond to press and release.
	// 入力アクションが押されたときと離されたときに対応するトリガーイベントを定義するために使用される。
	UPROPERTY(EditDefaultsOnly)
	ETriggerEvent ReleaseEventType = ETriggerEvent::Completed;
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

	// Unified input actions list (native + ability).
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "InputAction"))
	TArray<FMWInputAction> InputActions;
};

UCLASS(BlueprintType, Const)
class UMWInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	// Unified API
	const UInputAction* FindInputActionForTag(const FGameplayTag& IMCTag, const FGameplayTag& InputActionTag) const;
	bool GetInputActionsForTag(const FGameplayTag& IMCTag, TArray<FMWInputAction>& OutActions) const;
	bool GetInputActionForTag(const FGameplayTag& IMCTag, const FGameplayTag& InputActionTag, FMWInputAction& OutAction) const;

	// Backward compatible API (temporary)
	UE_DEPRECATED(5.6, "Use FindInputActionForTag")
	FORCEINLINE const UInputAction* FindNativeInputActionForTag(const FGameplayTag& IMCTag, const FGameplayTag& InputActionTag) const
	{
		return FindInputActionForTag(IMCTag, InputActionTag);
	}

	UE_DEPRECATED(5.6, "Use FindInputActionForTag")
	FORCEINLINE const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& IMCTag, const FGameplayTag& InputActionTag) const
	{
		return FindInputActionForTag(IMCTag, InputActionTag);
	}

	UE_DEPRECATED(5.6, "Use GetInputActionsForTag")
	FORCEINLINE bool GetAbilityInputActionsForTag(const FGameplayTag& IMCTag, TArray<FMWInputAction>& OutActions) const
	{
		return GetInputActionsForTag(IMCTag, OutActions);
	}

	UE_DEPRECATED(5.6, "Use GetInputActionForTag")
	FORCEINLINE bool GetAbilityInputActionForTag(const FGameplayTag& IMCTag, const FGameplayTag& InputActionTag, FMWInputAction& OutAction) const
	{
		return GetInputActionForTag(IMCTag, InputActionTag, OutAction);
	}

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FGameplayTag, FMWInputActionContainer> InputActionsContainers;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "Mapping"))
	TMap<FGameplayTag, FMWInputMappingContextWithPriority> InputMappingContext;
};
