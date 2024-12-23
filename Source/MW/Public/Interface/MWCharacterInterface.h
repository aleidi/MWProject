#pragma once

#include "UObject/Interface.h"
#include "MWCharacterInterface.generated.h"

UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UMWCharacterInterface : public UInterface
{
	GENERATED_BODY()
};

class IMWCharacterInterface
{
	GENERATED_BODY()

public:
	virtual float GetNormalizedVelocity() const = 0;
	virtual bool GetIsMoving() const = 0;
	virtual bool GetCanNormalAtk() const = 0;

	virtual TWeakPtr<class FMWTargetSelector> GetTargetSelector() const;
};