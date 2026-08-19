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
	// 追加の入力データを渡すため、AbilitySpecInputReleasedより前に呼び出されます。
	virtual void OnAbilityInputReleasedDataPassed(const FMWAbilityInputActionPayload& Payload) = 0;
};