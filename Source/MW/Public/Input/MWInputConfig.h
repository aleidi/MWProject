// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "MW.h"
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
 *	入力アクションをゲームプレイ入力タグに対応付ける構造体。
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
	ETriggerEvent PressedTriggerEvent = ETriggerEvent::Triggered;

	UPROPERTY(EditDefaultsOnly)
	ETriggerEvent ReleasedTriggerEvent = ETriggerEvent::Completed;

	/* ゲームパッド入力用の設定であり、通常は必要ありません。
	   特殊なケースでは、Chordアクションによりゲームパッド入力のCompletedイベントが発生しない場合があります。 */
	UPROPERTY(EditDefaultsOnly)
	bool bBindCancelToReleasedEvent = false;
};

USTRUCT(BlueprintType)
struct FMWInputMappingContextWithPriority
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputMappingContext> Mapping;

	/* 優先度の高いマッピングは、優先度の低いマッピングを上書きします。 */
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

	// 所有者が使用する入力アクションの一覧。これらはゲームプレイタグに対応付けられ、手動でバインドする必要があります。
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "InputAction"))
	TArray<FMWInputAction> NativeInputActions;

	// 所有者が使用する入力アクションの一覧。これらは一致する入力タグを持つアビリティに自動的にバインドされます。
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "InputAction"))
	TArray<FMWInputAction> AbilityInputActions;
};

 /**
  * UMWInputConfig
  *
	*	入力設定プロパティを保持する変更不可のデータアセット。
	*	各UMWInputConfigインスタンスは、1つのIMCとそれに関連付けられた入力アクションを管理します。
  */
 UCLASS(BlueprintType, Const)
 class UMWInputConfig : public UDataAsset
 {
 	GENERATED_BODY()

 public:
 	const UInputAction* FindNativeInputActionForTag(const FGameplayTag& InputActionTag) const;
 	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputActionTag) const;
 	bool GetAbilityInputActions(TArray<FMWInputAction>& OutActions) const;

 public:
	/* この設定が所有する入力マッピングコンテキスト。 */
 	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
 	FMWInputMappingContextWithPriority InputMappingContext;

	/* この設定が所有する入力アクション。 */
 	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
 	FMWInputActionContainer InputActionsContainer;
 };
