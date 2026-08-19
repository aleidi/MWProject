#pragma once

#include "MW.h"
//#include "MWDefineCommon.generated.h"

template<typename T>
concept DerivedFromUObject = std::is_base_of_v<UObject, T>;

// 文字列生成
#define STRING_FORMAT(Text, ...) FString::Printf(Text, ##__VA_ARGS__)