// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "MW.h"
#include "Gamemode/MWGameModeBase.h"
#include "MWEntryGameMode.generated.h"

/**
 * AMWEntryGamemode
 *
 *	The base game mode class used by this project.
 */
UCLASS()
class AMWEntryGamemode : public AMWGameModeBase
{
	GENERATED_BODY()

public:
	AMWEntryGamemode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~AGameModeBase interface
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	/* This method is used for spawning default pawn which may ignore default pawn class. */
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

	/* Spawn default pawn function */
	virtual APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) override;
	virtual void InitGameState() override;
	//~End of AGameModeBase interface

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	FName MenuName;
};
