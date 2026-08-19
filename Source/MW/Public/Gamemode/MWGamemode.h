// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "MW.h"
#include "Gamemode/MWGameModeBase.h"
#include "MWGameMode.generated.h"

/**
 * AMWGameMode
 *
 *	本プロジェクトで使用するGameModeの基底クラスです。
 */
UCLASS()
class AMWGameMode : public AMWGameModeBase
{
	GENERATED_BODY()

public:
	AMWGameMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~AGameModeBaseインターフェース
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	/* DefaultPawnClassを使用せずにDefaultPawnを生成できる処理です。 */
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

	/* DefaultPawn生成処理。 */
	virtual APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) override;
	virtual void InitGameState() override;
	//~AGameModeBaseインターフェース終了

protected:
	virtual void BeginPlay() override;
};
