#pragma once

// Include
#include "CoreMinimal.h"
#include "EnhancedInputSubsystemInterface.h"

// Forward Declare

// Macro

/*
 * @class UMWInputUtility
 * 
 * @brief Input Utility implements some convenient static functions
 *        to help process input related logic.
 *
 * @note
 */
class UMWInputUtility
{
public:
	static void EnableMappingContext(APlayerController* PC, const FName& Tag, const FModifyContextOptions& Options = FModifyContextOptions());

	static void DisableMappingContext(APlayerController* PC, const FName& Tag, const FModifyContextOptions& Options = FModifyContextOptions());
};