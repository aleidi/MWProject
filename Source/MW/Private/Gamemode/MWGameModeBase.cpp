#include "Gamemode/MWGameModeBase.h"

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
}
