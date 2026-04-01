#include "Gamemode/MWGameModeBase.h"

#include "Data/MWGameplayData.h"
#include "Gameplay/MWGameplayTags.h"
#include "Input/MWInputConfig.h"
#include "Input/MWInputUtility.h"
#include "Kismet/GameplayStatics.h"
#include "MWGameSingleton.h"

AMWGameModeBase::FOnGameInit AMWGameModeBase::OnGameInit;
AMWGameModeBase::FOnGameBeginPlay AMWGameModeBase::OnGameBeginPlay;

AMWGameModeBase::AMWGameModeBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void AMWGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	if (OnGameInit.IsBound())
	{
		OnGameInit.Broadcast(this);
	}
}

UClass* AMWGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	// Avoid to spawn default pawn
	return nullptr;
}

void AMWGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	if (OnGameBeginPlay.IsBound())
	{
		OnGameBeginPlay.Broadcast(this);
	}

	// Enable Basic Input Mapping Context
	const UMWGameplayData* data = GET_MWSINGLETON()->GetGameplayData();
	UMWInputUtility::EnableMappingContext(UGameplayStatics::GetPlayerController(this, 0), data->FindInputConfig(MWGameplayTags::IMC_Basic));
}