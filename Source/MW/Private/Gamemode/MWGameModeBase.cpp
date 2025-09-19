#include "Gamemode/MWGameModeBase.h"
#include "Gameplay/MWPartyManager.h"
#include "Input/MWInputUtility.h"
#include "Kismet/GameplayStatics.h"
#include "Gameplay/MWGameplayTags.h"

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

	// Initialize party and team
	CreateParty();

	// Enable Basic Input Mapping Context
	UMWInputUtility::EnableMappingContext(UGameplayStatics::GetPlayerController(this, 0), MWGameplayTags::IMC_Basic);
}

void AMWGameModeBase::CreateParty()
{
	auto* partyMng = UMWPartyManager::Get(this);

	check(partyMng);

	partyMng->InitializeParty();
}
