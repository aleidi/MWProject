#pragma once

#include "Data/Character/MWCharacterDataTypes.h"
#include "Data/MWPrimaryAsset.h"
#include "MWJobClassData.generated.h"

UCLASS()
class UMWJobClassData : public UMWPrimaryAsset
{
	GENERATED_BODY()

public:
	UMWJobClassData();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 Id = -1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	FMWCharacterAnimData JobAnimData;
};