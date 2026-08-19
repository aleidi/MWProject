// Copyright Epic Games, Inc. All Rights Reserved.

#include "Player/MWPlayerState.h"
#include "MWLogChannels.h"
#include "Controller/MWPlayerController.h"
#include "GameplayAbility/MWAbilitySystemComponent.h"
#include "Components/GameFrameworkComponentManager.h"
#include "GameplayAbility/MWAbilitySet.h"

//@TODO: 他要素へ依存せずPawn Dataを取得できるよう責務を分離
#include "Gamemode/MWGameMode.h"

AMWPlayerState::AMWPlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void AMWPlayerState::Reset()
{
	Super::Reset();
}

void AMWPlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);

	//@TODO: ステータスをコピー
}

AMWPlayerController* AMWPlayerState::GetMWPlayerController() const
{
	return Cast<AMWPlayerController>(GetOwner());
}

void AMWPlayerState::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void AMWPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}
