#pragma once

#include "CoreMinimal.h"
//#include "MWDefineCommon.generated.h"

template<typename T>
concept DerivedFromUObject = std::is_base_of_v<UObject, T>;