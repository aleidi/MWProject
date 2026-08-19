// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "AttributeSet.h"
#include "MWAttributeSet.generated.h"

class AActor;
class UMWAbilitySystemComponent;
class UObject;
class UWorld;
struct FGameplayEffectSpec;

/**
 * Attributeの参照と初期化に使用するHelper関数群を定義するMacroです。
 *
 * 次のMacro呼び出しにより、
 *		ATTRIBUTE_ACCESSORS(UMWHealthSet, Health)
 * 以下の関数を生成します：
 *		static FGameplayAttribute GetHealthAttribute();
 *		float GetHealth() const;
 *		void SetHealth(float NewVal);
 *		void InitHealth(float NewVal);
 */
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/** 
 * Attributeイベントを通知するDelegateです。クライアントでは一部の引数がnullの場合があります。
 * @param EffectInstigator イベントを最初に発生させたActor。
 * @param EffectCauser 変更を直接発生させたActor。
 * @param EffectSpec 変更に使用した完全なEffectSpec。
 * @param EffectMagnitude Clamp前の値。
 * @param OldValue 変更前のAttribute値。
 * @param NewValue 変更後のAttribute値。
*/
DECLARE_MULTICAST_DELEGATE_SixParams(FMWAttributeEvent, AActor* /*EffectInstigator*/, AActor* /*EffectCauser*/, const FGameplayEffectSpec* /*EffectSpec*/, float /*EffectMagnitude*/, float /*OldValue*/, float /*NewValue*/);

/**
 * UMWAttributeSet
 *
 *	本プロジェクトで使用するAttributeSetの基底クラスです。
 */
UCLASS()
class MW_API UMWAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:

	UMWAttributeSet();

	UWorld* GetWorld() const override;

	UMWAbilitySystemComponent* GetMWAbilitySystemComponent() const;
};
