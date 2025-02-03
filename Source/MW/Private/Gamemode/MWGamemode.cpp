// Copyright Epic Games, Inc. All Rights Reserved.

#include "Gamemode//MWGameMode.h"
#include "MWLogChannels.h"
#include "Controller/MWPlayerController.h"
#include "Player/MWPlayerState.h"
#include "Character/MWCharacter.h"
#include "Character/MWPawnData.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "System/MWAssetManager.h"
#include "Data/MWMasterData.h"

AMWGameMode::AMWGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	//GameStateClass = AMWGameState::StaticClass();
	//GameSessionClass = AMWGameSession::StaticClass();
	PlayerControllerClass = AMWPlayerController::StaticClass();
	//ReplaySpectatorPlayerControllerClass = AMWReplayPlayerController::StaticClass();
	PlayerStateClass = AMWPlayerState::StaticClass();
	DefaultPawnClass = AMWCharacter::StaticClass();
	//HUDClass = AMWHUD::StaticClass();
}

void AMWGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
}

void AMWGameMode::InitGameState()
{
	Super::InitGameState();
}

UClass* AMWGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	if (auto* data = UMWAssetManager::Get().GetMasterData())
	{
		if (auto pawn = UMWAssetManager::Get().GetSubclass(data->DefaultPawn))
		{
			return pawn;
		}
	}

	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

APawn* AMWGameMode::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform)
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = GetInstigator();
	SpawnInfo.ObjectFlags |= RF_Transient;	// Never save the default player pawns into a map.
	SpawnInfo.bDeferConstruction = true;

	if (UClass* PawnClass = GetDefaultPawnClassForController(NewPlayer))
	{
		if (APawn* spawned_pawn = GetWorld()->SpawnActor<APawn>(PawnClass, SpawnTransform, SpawnInfo))
		{

			spawned_pawn->FinishSpawning(SpawnTransform);

			return spawned_pawn;
		}
		else
		{
			UE_LOG(LogMW, Error, TEXT("Game mode was unable to spawn Pawn of class [%s] at [%s]."), *GetNameSafe(PawnClass), *SpawnTransform.ToHumanReadableString());
		}
	}
	else
	{
		UE_LOG(LogMW, Error, TEXT("Game mode was unable to spawn Pawn due to NULL pawn class."));
	}

	return nullptr;
}
