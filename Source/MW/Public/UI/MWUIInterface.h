#pragma once

#include "UObject/Interface.h"
#include "MWUIInterface.generated.h"

UINTERFACE()
class UMWUIInterface : public UInterface
{
	GENERATED_BODY()
};

class IMWUIInterface
{
	GENERATED_BODY()

public:
	virtual void PlayOpenAnim() = 0;
	virtual void PlayCloseAnim(TFunction<void()> Callback = nullptr) = 0;
};