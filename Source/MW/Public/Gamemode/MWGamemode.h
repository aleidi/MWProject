// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MWGameMode.generated.h"


class UMWPawnData;

/**
 * AMWGameMode
 *
 *	The base game mode class used by this project.
 */
UCLASS(Config = Game, Meta = (ShortTooltip = "The base game mode class used by this project."))
class AMWGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	AMWGameMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~AGameModeBase interface
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	/* This method is used for spawning default pawn which may ignore default pawn class. */
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

	/* Spawn default pawn function */
	virtual APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) override;
	virtual void InitGameState() override;
	//~End of AGameModeBase interface

protected:
	UPROPERTY(EditAnywhere,  Category = "Data")
	TObjectPtr<UMWPawnData> DefaultPawnData;
};
