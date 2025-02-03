// Copyright Epic Games, Inc. All Rights Reserved.

#include "Player/MWPlayerState.h"
#include "MWLogChannels.h"
#include "Controller/MWPlayerController.h"
#include "GameplayAbility/MWAbilitySystemComponent.h"
#include "Components/GameFrameworkComponentManager.h"
#include "GameplayAbility/MWAbilitySet.h"

//@TODO: Would like to isolate this a bit better to get the pawn data in here without this having to know about other stuff
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

	//@TODO: Copy stats
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
