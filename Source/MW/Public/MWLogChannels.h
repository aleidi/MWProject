#pragma once

#include "CoreMinimal.h"
#include "Engine/Engine.h"
#include "Logging/LogMacros.h"

class UObject;

DEFINE_LOG_CATEGORY_STATIC(DefaultLog, All, All)
DEFINE_LOG_CATEGORY_STATIC(ErrorCategory, All, All)

DECLARE_LOG_CATEGORY_EXTERN(LogMW, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogMWAbilitySystem, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogMWComponent, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogMWAnimNotify, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogMWSkill, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogMWBattle, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogMWUI, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogMWEntity, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogMWInput, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogMWCommon, Log, All);

#define __STRINGIFY_HELPER(x) #x
#define __STRINGIFY(x) __STRINGIFY_HELPER(x)
#ifdef _MSC_VER
#	define __FUNC_NAME__	__FUNCTION__
#	else
#	define __FUNC_NAME__	"" //__func__ 
#endif

#define FUNC_INFO TEXT("(") TEXT(__FUNCTION__) TEXT("(") __STRINGIFY(__LINE__) TEXT("))")

static inline FString GetPIEClientID()
{
#if WITH_EDITOR
    return FString::Printf(TEXT("[View:%d]"), static_cast<int32>(UE::GetPlayInEditorID()));
#else    
    return FString(TEXT(""));
#endif
}

static IConsoleVariable* MWLOGVerbose = IConsoleManager::Get().RegisterConsoleVariable(TEXT("Log.Verbose"), 0, TEXT("Logs include the function name and line number.\n0-Clear\n1-WithFunc\n2-WIthView"), ECVF_Default);


#define MW_LOG_TRACE(LogCategory, LogType) \
    { \
        int32 VerboseLogLevel = MWLOGVerbose ? MWLOGVerbose->GetInt() : 1; \
        switch (VerboseLogLevel)\
        { \
            case 2: UE_LOG(LogCategory, LogType, TEXT("%s%s"), *GetPIEClientID(), FUNC_INFO); break; \
            case 1: UE_LOG(LogCategory, LogType, TEXT("%s%s"), *GetPIEClientID(), FUNC_INFO); break; \
            default: UE_LOG(LogCategory, LogType, FUNC_INFO); break; \
        } \
    }

#define MW_LOG(LogCategory, LogType, Message, ...) \
    { \
        int32 VerboseLogLevel = MWLOGVerbose ? MWLOGVerbose->GetInt() : 1; \
        switch (VerboseLogLevel)\
        { \
            case 2: UE_LOG(LogCategory, LogType, TEXT("%s%s " Message), *GetPIEClientID(), FUNC_INFO, ##__VA_ARGS__); break; \
            case 1: UE_LOG(LogCategory, LogType, TEXT("%s " Message), FUNC_INFO, ##__VA_ARGS__); break; \
            default: UE_LOG(LogCategory, LogType, Message, ##__VA_ARGS__); break; \
        } \
    }

#define MW_LOG_DEFAULT(Message, ...)             MW_LOG(DefaultLog, Log, Message, ##__VA_ARGS__)
#define MW_LOG_WARNING(Message, ...)             MW_LOG(ErrorCategory, Warning, Message, ##__VA_ARGS__)
#define MW_LOG_ERROR(Message, ...)               MW_LOG(ErrorCategory, Error, Message, ##__VA_ARGS__)


#define MW_LOG_WITH_SCREEN(Message, ...) MW_LOG_DEFAULT(Message, ##__VA_ARGS__); \
GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(Message, ##__VA_ARGS__))

#define MW_LOG_ONLY_SCREEN(Key,TimeToDisplay, Message, ...) \
GEngine->AddOnScreenDebugMessage(Key, TimeToDisplay, FColor::Red, FString::Printf(Message, ##__VA_ARGS__))

