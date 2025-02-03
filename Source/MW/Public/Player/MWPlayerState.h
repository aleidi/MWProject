// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MWPlayerState.generated.h"


class AMWPlayerController;

/**
 * AMWPlayerState
 *
 *	Base player state class used by this project.
 */
UCLASS(Config = Game)
class MW_API AMWPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AMWPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "MW|PlayerState")
	AMWPlayerController* GetMWPlayerController() const;

	//~AActor interface
	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;
	//~End of AActor interface

	//~APlayerState interface
	virtual void Reset() override;
	virtual void CopyProperties(APlayerState* PlayerState) override;
	//~End of APlayerState interface
};
