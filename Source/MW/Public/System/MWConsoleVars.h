#pragma once

#include "HAL/IConsoleManager.h"

/**
 * MWConsoleVars
 *
 * Centralized management of all Console Variables
 * How to use：
 *   #include "Common/MWConsoleVars.h"
 *   if (MWConsoleVars::bShowSkillDebug->GetValueOnGameThread()) { ... }
 */
namespace MWConsoleVars
{
	// ==== Ability / Skill ====
#pragma region Ability Skill

	/** 是否在屏幕上显示 Skill Ability 的调试信息 */
	extern MW_API TAutoConsoleVariable<bool> CVarShowSkillDebug;

#pragma endregion

	// ==== Input ====
#pragma region Input
	/** 是否在屏幕上显示 Input 的调试信息 */
	extern MW_API TAutoConsoleVariable<bool> CVarShowInputDebug;

	/** 是否在屏幕上显示 PlayerControlller 的调试信息 */
	extern MW_API TAutoConsoleVariable<bool> CVarShowPlayerControllerDebug;
#pragma endregion
}