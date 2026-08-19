// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "MW.h"
#include "GameFramework/GameModeBase.h"
#include "MWGameModeBase.generated.h"

/**
 * AMWGameModeBase
 *
 *	本プロジェクトで使用するGameModeの基底クラスです。
 */
UCLASS(Abstract,Config = Game, Meta = (ShortTooltip = "The base game mode class used by this project."))
class AMWGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMWGameModeBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~AGameModeBaseインターフェース
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	/* DefaultPawnClassを使用せずにDefaultPawnを生成できる処理です。 */
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
	//~AGameModeBaseインターフェース終了

protected:
	virtual void BeginPlay() override;

public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameInit, AGameModeBase*);
	static FOnGameInit OnGameInit;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameBeginPlay, AGameModeBase*);
	static FOnGameBeginPlay OnGameBeginPlay;
};
