#pragma once

// Include Header
#include "MWEntity.h"
#include "MWCharacterEntity.generated.h"

// Forward Declare

// Define

/*
 * @class UMWCharacterEntity
 * 
 * @brief
 *
 * @note
 */
UCLASS()
class UMWCharacterEntity : public UMWEntity
{
	GENERATED_BODY()

public:
	virtual UClass* GetActorClass() const override;
};