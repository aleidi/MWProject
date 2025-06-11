// Copyright Epic Games, Inc. All Rights Reserved.

#include "Gamemode/MWEntryGameMode.h"
#include "MWLogChannels.h"
#include "Controller/MWPlayerController.h"
#include "Player/MWPlayerState.h"
#include "Character/MWCharacter.h"
#include "Character/MWPawnData.h"
#include "Kismet/GameplayStatics.h"
#include "System/MWAssetManager.h"
#include "UI/MWUIManager.h"

AMWEntryGamemode::AMWEntryGamemode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	//GameStateClass = AMWGameState::StaticClass();
	//GameSessionClass = AMWGameSession::StaticClass();
	//PlayerControllerClass = AMWPlayerController::StaticClass();
	//ReplaySpectatorPlayerControllerClass = AMWReplayPlayerController::StaticClass();
	//PlayerStateClass = AMWPlayerState::StaticClass();
	//DefaultPawnClass = AMWCharacter::StaticClass();
	//HUDClass = AMWHUD::StaticClass();
}

void AMWEntryGamemode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
}

void AMWEntryGamemode::InitGameState()
{
	Super::InitGameState();
}

void AMWEntryGamemode::BeginPlay()
{
	Super::BeginPlay();

	UGameInstance* gi = GetWorld()->GetGameInstance();
	
	if (auto* uiMng = gi->GetSubsystem<UMWUIManager>())
	{
		uiMng->OpenUI(MenuName, true);
	}
}

UClass* AMWEntryGamemode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

APawn* AMWEntryGamemode::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform)
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
