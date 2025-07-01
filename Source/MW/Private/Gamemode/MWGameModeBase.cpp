#include "Gamemode/MWGameModeBase.h"
#include "Gameplay/MWPartyManager.h"

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

void AMWGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	if (OnGameBeginPlay.IsBound())
	{
		OnGameBeginPlay.Broadcast(this);
	}

	// Initialize party and team
	CreateParty();
}

void AMWGameModeBase::CreateParty()
{
	auto* partyMng = GetWorld()->GetGameInstance()->GetSubsystem<UMWPartyManager>();

	check(partyMng);

	partyMng->InitializeParty();
}
