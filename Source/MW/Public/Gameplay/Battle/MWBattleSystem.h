// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Define/MWDefineGameplay.h"
#include "GameplayTagContainer.h"
#include "MWBattleSystem.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class MW_API UMWBattleSystem : public UObject
{
	GENERATED_BODY()

public:

public:
	void Initialize() {}
	void Deinitialize() {}

	UFUNCTION(BlueprintPure, Category = "Manager", meta = (WorldContext="WorldContext", DisplayName = "GetBattleSystem"))
	static UMWBattleSystem* Get(const UObject* WorldContext);

};
