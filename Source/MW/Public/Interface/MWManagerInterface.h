#pragma once

#include "UObject/Interface.h"
#include "MWManagerInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UMWManagerInterface : public UInterface
{
	GENERATED_BODY()
};

class IMWManagerInterface
{
	GENERATED_BODY()

public:
	virtual void Initialize() = 0;
	virtual void Deinitialize() = 0;
};