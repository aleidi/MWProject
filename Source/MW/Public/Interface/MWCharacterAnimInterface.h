#pragma once

#include "UObject/Interface.h"
#include "MWCharacterAnimInterface.generated.h"

UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UMWCharacterAnimInterface : public UInterface
{
	GENERATED_BODY()
};

class IMWCharacterAnimInterface
{
	GENERATED_BODY()

public:
	virtual bool CastSkillAnim(UAnimMontage* MontageToPlay, float PlayRate, bool bForce) = 0;
};