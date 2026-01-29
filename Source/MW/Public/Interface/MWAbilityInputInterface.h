#pragma once

#include "UObject/Interface.h"
#include "MWAbilityInputInterface.generated.h"

struct FMWAbilityInputActionPayload;

UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UMWAbilityInputInterface : public UInterface
{
	GENERATED_BODY()
};

class IMWAbilityInputInterface
{
	GENERATED_BODY()

public:
	// Called before AbilitySpecInputReleased to pass extra input data.
	virtual void OnAbilityInputReleasedDataPassed(const FMWAbilityInputActionPayload& Payload) = 0;
};