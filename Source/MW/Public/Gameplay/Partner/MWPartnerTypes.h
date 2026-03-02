#pragma once

#include "MWPartnerTypes.generated.h"

UENUM(BlueprintType)
enum class EMWPartnerRole : uint8
{
	Master,
	Follower,
	NotSet
};