#pragma once

#include "Engine/DataTable.h"
#include "MWGameDataTypes.generated.h"

class UMWCharacterData;

USTRUCT(BlueprintType)
struct FMWCharacterRegistryRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 Id = -1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UMWCharacterData> CharacterData;
};