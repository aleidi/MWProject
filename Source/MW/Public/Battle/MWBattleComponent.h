// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MWBattleComponent.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class MW_API UMWBattleComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMWBattleComponent();

#pragma region Skill
public:
	UFUNCTION(BlueprintCallable, Category = "Battle|Skill")
	bool TryCastSkill(int32 GroupId, int32 SkillId);

	UFUNCTION(BlueprintPure, Category = "Battle|Skill")
	bool CanCombo() const;

private:
#pragma endregion
};
