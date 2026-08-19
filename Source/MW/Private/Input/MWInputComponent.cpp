// Copyright Epic Games, Inc. All Rights Reserved.

#include "Input/MWInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(MWInputComponent)

class UMWInputConfig;

UMWInputComponent::UMWInputComponent(const FObjectInitializer& ObjectInitializer)
{
}

void UMWInputComponent::AddInputMappings(const UMWInputConfig* InputConfig, UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const
{
	check(InputConfig);
	check(InputSubsystem);

	// 必要に応じてInput Config追加時のカスタム処理を実装
}

void UMWInputComponent::RemoveInputMappings(const UMWInputConfig* InputConfig, UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const
{
	check(InputConfig);
	check(InputSubsystem);

	// 必要に応じて追加済みInput Mappingの削除処理を実装
}

void UMWInputComponent::RemoveBinds(TArray<uint32>& BindHandles)
{
	for (uint32 Handle : BindHandles)
	{
		RemoveBindingByHandle(Handle);
	}
	BindHandles.Reset();
}
