#pragma once

#include "Engine/Engine.h"

/**
 * UEDebugUtils
 *
 * Unreal Engine向けのプロジェクト非依存な画面DebugMessage Macroです。
 * プロジェクト固有の依存関係がなく、任意のUEプロジェクトで利用できます。
 *
 * Shipping Buildでは全Macroを完全に除去します：
 *   - 引数も評価されないため、副作用と実行Costは発生しません。
 *
 * Macro一覧：
 *
 *   UE_SCREEN_PRINT          - bool式で表示を制御
 *   UE_SCREEN_PRINT_CVAR     - TAutoConsoleVariable<bool>で表示を制御
 *   UE_SCREEN_PRINT_ALWAYS   - 条件なしで常時表示（Shippingでは除去）
 *
 * 使用例：
 *   // bool条件
 *   UE_SCREEN_PRINT(bMyFlag, 5.f, FColor::Green, TEXT("Val: %d"), MyInt);
 *
 *   // CVar条件
 *   UE_SCREEN_PRINT_CVAR(MyCVar, 0.f, FColor::Yellow, TEXT("Tick: %.2f"), Delta);
 *
 *   // 常時表示（一時調査用。Shipping前に削除）
 *   UE_SCREEN_PRINT_ALWAYS(5.f, FColor::Red, TEXT("Reached here"));
 */

#if !UE_BUILD_SHIPPING

	/**
	 * bool式で制御する画面DebugMessageを表示します。
	 * GEngineがnull、またはbEnabledがfalseの場合は何もしません。
	 *
	 * @param bEnabled 実行時に評価するbool式。
	 * @param Time 表示時間（秒）。0.fの場合は1Frameのみ。
	 * @param Color Text色（FColor）。
	 * @param Format Format文字列。const TCHAR Literal（TEXT(...)）を指定します。
	 * @param ... Format引数。
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
	 * TAutoConsoleVariable<bool>で制御する画面DebugMessageを表示します。
	 * GEngineがnull、またはCVar値がfalseの場合は何もしません。
	 *
	 * @param CVar TAutoConsoleVariable<bool> Instance（Pointerではありません）。
	 * @param Time 表示時間（秒）。0.fの場合は1Frameのみ。
	 * @param Color Text色（FColor）。
	 * @param Format Format文字列。const TCHAR Literal（TEXT(...)）を指定します。
	 * @param ... Format引数。
	 */
	#define UE_SCREEN_PRINT_CVAR(CVar, Time, Color, Format, ...) \
		UE_SCREEN_PRINT((CVar).GetValueOnGameThread(), Time, Color, Format, ##__VA_ARGS__)

	/**
	 * 条件なしで画面DebugMessageを表示します。
	 * 一時調査専用のため、Release前に削除します。
	 *
	 * @param Time 表示時間（秒）。0.fの場合は1Frameのみ。
	 * @param Color Text色（FColor）。
	 * @param Format Format文字列。const TCHAR Literal（TEXT(...)）を指定します。
	 * @param ... Format引数。
	 */
	#define UE_SCREEN_PRINT_ALWAYS(Time, Color, Format, ...) \
		do \
		{ \
			if (GEngine) \
			{ \
				GEngine->AddOnScreenDebugMessage(-1, (Time), (Color), FString::Printf(Format, ##__VA_ARGS__)); \
			} \
		} while (false)

#else // UE_BUILD_SHIPPINGでは全Macroを空に展開します。

	#define UE_SCREEN_PRINT(bEnabled, Time, Color, Format, ...)
	#define UE_SCREEN_PRINT_CVAR(CVar, Time, Color, Format, ...)
	#define UE_SCREEN_PRINT_ALWAYS(Time, Color, Format, ...)

#endif // !UE_BUILD_SHIPPING
