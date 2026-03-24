#include "MWLogChannels.h"

DEFINE_LOG_CATEGORY(LogMW);
DEFINE_LOG_CATEGORY(LogMWAbilitySystem);
DEFINE_LOG_CATEGORY(LogMWAnimNotify);
DEFINE_LOG_CATEGORY(LogMWBattle);
DEFINE_LOG_CATEGORY(LogMWCommon);
DEFINE_LOG_CATEGORY(LogMWComponent);
DEFINE_LOG_CATEGORY(LogMWData);
DEFINE_LOG_CATEGORY(LogMWInput);
DEFINE_LOG_CATEGORY(LogMWSkill);
DEFINE_LOG_CATEGORY(LogMWUI);

IConsoleVariable* MWLOGVerbose = IConsoleManager::Get().RegisterConsoleVariable(
    TEXT("MWLog.Verbose"),
    0,
    TEXT("Logs include the function name and line number.\n0-Clear\n1-WithFunc\n2-WIthView"),
    ECVF_Default
);