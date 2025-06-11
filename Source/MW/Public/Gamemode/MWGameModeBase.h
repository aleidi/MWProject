// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MWGameModeBase.generated.h"

/**
 * AMWGameModeBase
 *
 *	The base game mode class used by this project.
 */
UCLASS(Abstract,Config = Game, Meta = (ShortTooltip = "The base game mode class used by this project."))
class AMWGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMWGameModeBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~AGameModeBase interface
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	//~End of AGameModeBase interface

protected:
	virtual void BeginPlay() override;

public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameInit, AGameModeBase*);
	static FOnGameInit OnGameInit;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameBeginPlay, AGameModeBase*);
	static FOnGameBeginPlay OnGameBeginPlay;
};
