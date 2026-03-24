#pragma once

#include "CoreMinimal.h"
//#include "MWDefineCommon.generated.h"

template<typename T>
concept DerivedFromUObject = std::is_base_of_v<UObject, T>;

//String Build
#define STRING_FORMAT(Text, ...) FString::Printf(Text, ##__VA_ARGS__)

// Path
#define DATA_PATH TEXT("/Game/Data")
#define CHARACTER_DATA_PATH TEXT("/Character/DT_CharacterRegistryTable")