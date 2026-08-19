#pragma once

#include "HAL/IConsoleManager.h"

/**
 * MWConsoleVars
 *
 * 全ConsoleVariableを一元管理します。
 * 使用例：
 *   #include "Common/MWConsoleVars.h"
 *   if (MWConsoleVars::bShowSkillDebug->GetValueOnGameThread()) { ... }
 */
namespace MWConsoleVars
{
	// ==== アビリティ／スキル ====
#pragma region Ability Skill

	/** Skill Abilityのデバッグ情報を画面に表示するか。 */
	extern MW_API TAutoConsoleVariable<bool> CVarShowSkillDebug;

#pragma endregion

	// ==== 入力 ====
#pragma region Input
	/** Inputのデバッグ情報を画面に表示するか。 */
	extern MW_API TAutoConsoleVariable<bool> CVarShowInputDebug;

	/** PlayerControllerのデバッグ情報を画面に表示するか。 */
	extern MW_API TAutoConsoleVariable<bool> CVarShowPlayerControllerDebug;
#pragma endregion
}