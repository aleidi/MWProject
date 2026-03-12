#pragma once

#include "Engine/Engine.h"

/**
 * UEDebugUtils
 *
 * Project-agnostic screen debug message macros for Unreal Engine.
 * No project-specific dependencies — safe to copy into any UE project.
 *
 * All macros are stripped entirely in Shipping builds:
 *   - Arguments are NOT evaluated in Shipping (no side effects, no overhead).
 *
 * Macro families:
 *
 *   UE_SCREEN_PRINT          — Controlled by a plain bool expression
 *   UE_SCREEN_PRINT_CVAR     — Controlled by a TAutoConsoleVariable<bool>
 *   UE_SCREEN_PRINT_ALWAYS   — Always prints (no guard), Shipping stripped
 *
 * Usage:
 *   // Bool guard
 *   UE_SCREEN_PRINT(bMyFlag, 5.f, FColor::Green, TEXT("Val: %d"), MyInt);
 *
 *   // CVar guard
 *   UE_SCREEN_PRINT_CVAR(MyCVar, 0.f, FColor::Yellow, TEXT("Tick: %.2f"), Delta);
 *
 *   // Always (temporary investigation, remove before shipping)
 *   UE_SCREEN_PRINT_ALWAYS(5.f, FColor::Red, TEXT("Reached here"));
 */

#if !UE_BUILD_SHIPPING

	/**
	 * Prints a screen debug message controlled by a plain bool expression.
	 * No-op if GEngine is null or bEnabled evaluates to false.
	 *
	 * @param bEnabled   bool expression evaluated at runtime
	 * @param Time       Display duration in seconds; 0.f = one frame only
	 * @param Color      Text color (FColor)
	 * @param Format     Format string, must be a const TCHAR literal (TEXT(...))
	 * @param ...        Format arguments
	 */
	#define UE_SCREEN_PRINT(bEnabled, Time, Color, Format, ...) \
		do \
		{ \
			if (GEngine && (bEnabled)) \
			{ \
				GEngine->AddOnScreenDebugMessage(-1, (Time), (Color), FString::Printf(Format, ##__VA_ARGS__)); \
			} \
		} while (false)

	/**
	 * Prints a screen debug message controlled by a TAutoConsoleVariable<bool>.
	 * No-op if GEngine is null or the CVar value is false.
	 *
	 * @param CVar       TAutoConsoleVariable<bool> instance (not a pointer)
	 * @param Time       Display duration in seconds; 0.f = one frame only
	 * @param Color      Text color (FColor)
	 * @param Format     Format string, must be a const TCHAR literal (TEXT(...))
	 * @param ...        Format arguments
	 */
	#define UE_SCREEN_PRINT_CVAR(CVar, Time, Color, Format, ...) \
		UE_SCREEN_PRINT((CVar).GetValueOnGameThread(), Time, Color, Format, ##__VA_ARGS__)

	/**
	 * Unconditionally prints a screen debug message.
	 * Intended for temporary investigation only — remove before release.
	 *
	 * @param Time       Display duration in seconds; 0.f = one frame only
	 * @param Color      Text color (FColor)
	 * @param Format     Format string, must be a const TCHAR literal (TEXT(...))
	 * @param ...        Format arguments
	 */
	#define UE_SCREEN_PRINT_ALWAYS(Time, Color, Format, ...) \
		do \
		{ \
			if (GEngine) \
			{ \
				GEngine->AddOnScreenDebugMessage(-1, (Time), (Color), FString::Printf(Format, ##__VA_ARGS__)); \
			} \
		} while (false)

#else // UE_BUILD_SHIPPING — all macros expand to nothing

	#define UE_SCREEN_PRINT(bEnabled, Time, Color, Format, ...)
	#define UE_SCREEN_PRINT_CVAR(CVar, Time, Color, Format, ...)
	#define UE_SCREEN_PRINT_ALWAYS(Time, Color, Format, ...)

#endif // !UE_BUILD_SHIPPING