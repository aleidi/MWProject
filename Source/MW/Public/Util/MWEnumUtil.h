#pragma once

#include "UOBject/UObjectGlobals.h"
#include "UObject/Class.h"

#define ENUM_TO_STRING(EnumType, EnumValue) _EnumToString<EnumType>(#EnumType, EnumValue)
#define STRING_TO_ENUM(EnumType, EnumString) _StringToEnum<EnumType>(#EnumType, EnumString)

static UEnum* GetEnumPointer(const FString& enumName)
{
	return FindObject<UEnum>(nullptr, *FString::Printf(TEXT("/Script/MW.%s"), *enumName));
}

template<typename T>
static FString _EnumToString(const FString& EnumName, const T Value)
{
	UEnum* EnumPtr = GetEnumPointer(EnumName);

	return EnumPtr ? EnumPtr->GetNameStringByValue(static_cast<int64>(Value)) : TEXT("null");
}

template<typename T>
static T _StringToEnum(const FString& EnumName, const FString& EnumNameStr)
{
	UEnum* EnumPtr = GetEnumPointer(EnumName);
	if (EnumPtr == nullptr)
	{
		return (T)0;
	}

	return (T)EnumPtr->GetValueByNameString(*EnumNameStr);
}

template<typename TEnum> requires std::is_enum_v<TEnum>
static FString EnumToString(TEnum Value)
{
	UEnum* Enum = StaticEnum<TEnum>();

	if (!Enum)
	{
		return TEXT("null");
	}

	return Enum->GetNameStringByValue(static_cast<int64>(Value));
}

template<typename TEnum> requires std::is_enum_v<TEnum>
static FName EnumToName(TEnum Value)
{
	UEnum* Enum = StaticEnum<TEnum>();
	if (!Enum)
	{
		return TEXT("null");
	}
	return Enum->GetNameByValue(static_cast<int64>(Value));
}

template<typename TEnum>
static FORCEINLINE TArray<FString> EnumToStringArr()
{
	static_assert(TIsEnum<TEnum>::Value);

	TArray<FString> returnArr;

	if (UEnum* findEnum = StaticEnum<TEnum>())
	{
		for (int i = 0; i < findEnum->NumEnums(); i++)
		{
			returnArr.Add(findEnum->GetNameStringByIndex(i));
		}

		returnArr.Pop();
	}
	else
	{
		UE_DEBUG_BREAK();
	}

	return returnArr;
}


template<typename TEnum>
static FORCEINLINE TArray<TEnum> GetEnumValueArr()
{
	static_assert(TIsEnum<TEnum>::Value);

	TArray<TEnum> returnArr;

	if (UEnum* findEnum = StaticEnum<TEnum>())
	{
		for (int i = 0; i < findEnum->NumEnums(); i++)
		{
			returnArr.Add(static_cast<TEnum>(findEnum->GetValueByIndex(i)));
		}

		returnArr.Pop();
	}
	else
	{
		UE_DEBUG_BREAK();
	}

	return returnArr;
}