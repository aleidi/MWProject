// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "MW.h"
#include "GameFramework/PlayerState.h"
#include "MWPlayerState.generated.h"


class AMWPlayerController;

/**
 * AMWPlayerState
 *
 *	本プロジェクトで使用するPlayerStateの基底クラスです。
 */
UCLASS(Config = Game)
class MW_API AMWPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AMWPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "MW|PlayerState")
	AMWPlayerController* GetMWPlayerController() const;

	//~AActorインターフェース
	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;
	//~AActorインターフェース終了

	//~APlayerStateインターフェース
	virtual void Reset() override;
	virtual void CopyProperties(APlayerState* PlayerState) override;
	//~APlayerStateインターフェース終了
};
