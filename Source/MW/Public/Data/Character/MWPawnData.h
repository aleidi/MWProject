// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Engine/DataAsset.h"

#include "MWPawnData.generated.h"

class APawn;
class UMWAbilitySet;
class UMWAbilityTagRelationshipMapping;
class UMWCameraMode;
class UMWInputConfig;
class UObject;


/**
 * UMWPawnData
 *
 *	Non-mutable data asset that contains properties used to define a pawn.
 */
UCLASS(BlueprintType, Const, Meta = (DisplayName = "MW Pawn Data", ShortTooltip = "Data asset used to define a Pawn."))
class MW_API UMWPawnData : public UDataAsset
{
	GENERATED_BODY()

public:

	UMWPawnData(const FObjectInitializer& ObjectInitializer);

	FPrimaryAssetId GetPrimaryAssetId() const override;

public:

	// Class to instantiate for this pawn (should usually derive from AMWPawn or AMWCharacter).
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MW|Pawn")
	TSubclassOf<APawn> PawnClass;

	// Ability sets to grant to this pawn's ability system.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MW|Abilities")
	TArray<TObjectPtr<UMWAbilitySet>> AbilitySets;

	//// What mapping of ability tags to use for actions taking by this pawn
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MW|Abilities")
	//TObjectPtr<UMWAbilityTagRelationshipMapping> TagRelationshipMapping;

	// Input configuration used by player controlled pawns to create input mappings and bind input actions.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MW|Input")
	TObjectPtr<UMWInputConfig> InputConfig;

	//// Default camera mode used by player controlled pawns.
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MW|Camera")
	//TSubclassOf<UMWCameraMode> DefaultCameraMode;
};
