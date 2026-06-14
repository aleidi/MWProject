// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameFramework/Character.h"

#include "ModularCharacter.generated.h"

#define UE_API MODULARGAMEPLAYACTORS_API

class UObject;

/** Minimal class that supports extension by game feature plugins */
UCLASS(MinimalAPI, Blueprintable)
class AModularCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	//~ Begin AActor Interface
	UE_API virtual void PreInitializeComponents() override;
	UE_API virtual void BeginPlay() override;
	UE_API virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~ End AActor Interface

public:
	void SetCharacterId(int32 NewId) { CharacterId = NewId; }
	int32 GetCharacterId() const { return CharacterId; }

private:
	UPROPERTY(EditDefaultsOnly, Category = "Character", meta=(AllowPrivateAccess = "True"))
	int32 CharacterId = INDEX_NONE;
};

#undef UE_API
