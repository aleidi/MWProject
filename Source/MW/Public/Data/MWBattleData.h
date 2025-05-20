#pragma once

#include "Engine/DataAsset.h"
#include "MWBattleData.generated.h"

UCLASS(BlueprintType)
class UMWBattleSceneSetting : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	float FriendlyDistance = 100.f;

	UPROPERTY(EditDefaultsOnly)
	float FriendlySpace = 100.f;

	UPROPERTY(EditDefaultsOnly)
	float EnemyDistance = 100.f;

	UPROPERTY(EditDefaultsOnly)
	float EnemySpace = 100.f;
};