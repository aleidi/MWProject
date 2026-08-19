// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameFeatures/GameFeatureAction_WorldActionBase.h"

#include "Engine/GameInstance.h"
#include "Engine/World.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GameFeatureAction_WorldActionBase)

void UGameFeatureAction_WorldActionBase::OnGameFeatureActivating(FGameFeatureActivatingContext& Context)
{
	GameInstanceStartHandles.FindOrAdd(Context) = FWorldDelegates::OnStartGameInstance.AddUObject(this, 
		&UGameFeatureAction_WorldActionBase::HandleGameInstanceStart, FGameFeatureStateChangeContext(Context));

	// 初期化済みGameInstanceに関連付けられた全Worldへ追加
	for (const FWorldContext& worldContext : GEngine->GetWorldContexts())
	{
		if (Context.ShouldApplyToWorldContext(worldContext))
		{
			AddToWorld(worldContext, Context);
		}
	}
}

void UGameFeatureAction_WorldActionBase::OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context)
{
	FDelegateHandle* foundHandle = GameInstanceStartHandles.Find(Context);
	if (ensure(foundHandle))
	{
		FWorldDelegates::OnStartGameInstance.Remove(*foundHandle);
	}
	
}

void UGameFeatureAction_WorldActionBase::HandleGameInstanceStart(UGameInstance* GameInstance, FGameFeatureStateChangeContext ChangeContext)
{
	if (FWorldContext* worldContext = GameInstance->GetWorldContext())
	{
		if (ChangeContext.ShouldApplyToWorldContext(*worldContext))
		{
			AddToWorld(*worldContext, ChangeContext);
		}
	}
}

