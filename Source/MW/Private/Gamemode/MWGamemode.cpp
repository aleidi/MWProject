#include "Gamemode/MWGameMode.h"

#include "Character/MWCharacter.h"
#include "Controller/MWPlayerController.h"
#include "Data/MWGameplayData.h"
#include "MWGameSingleton.h"
#include "MWLogChannels.h"
#include "Pawn/MWPawnExtensionComponent.h"
#include "Player/MWPlayerState.h"

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

void AMWGameMode::BeginPlay()
{
	Super::BeginPlay();
}

UClass* AMWGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	if (auto* data = GET_MWSINGLETON()->GetGameplayData())
	{
		return data->DefaultPawn;
	}

	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

APawn* AMWGameMode::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform)
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = GetInstigator();
	SpawnInfo.ObjectFlags |= RF_Transient;	// Never save the default player pawns into a map.
	SpawnInfo.bDeferConstruction = true;

	if (UClass* pawnClass = GetDefaultPawnClassForController(NewPlayer))
	{
		if (APawn* spawnedPawn = GetWorld()->SpawnActor<APawn>(pawnClass, SpawnTransform, SpawnInfo))
		{
			if (UMWPawnExtensionComponent* pawnExtComp = UMWPawnExtensionComponent::FindPawnExtensionComponent(spawnedPawn))
			{
				pawnExtComp->SetPawnDataId(1);
			}

			spawnedPawn->FinishSpawning(SpawnTransform);

			return spawnedPawn;
		}
		else
		{
			UE_LOG(LogMW, Error, TEXT("Game mode was unable to spawn Pawn of class [%s] at [%s]."), *GetNameSafe(pawnClass), *SpawnTransform.ToHumanReadableString());
		}
	}
	else
	{
		UE_LOG(LogMW, Error, TEXT("Game mode was unable to spawn Pawn due to NULL pawn class."));
	}

	return nullptr;
}
