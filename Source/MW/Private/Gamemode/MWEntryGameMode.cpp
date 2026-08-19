#include "Gamemode/MWEntryGameMode.h"

#include "Character/MWCharacter.h"
#include "Controller/MWPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "MWLogChannels.h"
#include "Player/MWPlayerState.h"
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

	if (auto* uiMng = UMWUIManager::Get(this))
	{
		uiMng->OpenUI(MenuName, true);
	}

	APlayerController* pc = UGameplayStatics::GetPlayerController(this, 0);

	if(pc)
	{
		pc->SetShowMouseCursor(true);
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
	SpawnInfo.ObjectFlags |= RF_Transient;	// デフォルトPlayer Pawnはマップへ保存しない
	SpawnInfo.bDeferConstruction = true;

	if (UClass* PawnClass = GetDefaultPawnClassForController(NewPlayer))
	{
		if (APawn* spawnedPawn = GetWorld()->SpawnActor<APawn>(PawnClass, SpawnTransform, SpawnInfo))
		{

			spawnedPawn->FinishSpawning(SpawnTransform);

			return spawnedPawn;
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
