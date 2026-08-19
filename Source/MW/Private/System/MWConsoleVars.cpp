#include "System/MWConsoleVars.h"

namespace MWConsoleVars
{
	// ==== Ability／スキル ====
#pragma region Ability Skill

	TAutoConsoleVariable<bool> CVarShowSkillDebug(
		TEXT("mw.ShowDebug.Skill"),
		false,
		TEXT("Should display debug information for skill ability.\n")
		TEXT("  0 = Disable (Default)\n")
		TEXT("  1 = Enable"),
		ECVF_Cheat
	);

#pragma endregion

	// ==== 入力 ====
#pragma region Input
	TAutoConsoleVariable<bool> CVarShowInputDebug(
		TEXT("mw.ShowDebug.Input"),
		false,
		TEXT("Should display debug information for input.\n")
		TEXT("  0 = Disable (Default)\n")
		TEXT("  1 = Enable"),
		ECVF_Cheat
	);

	TAutoConsoleVariable<bool> CVarShowPlayerControllerDebug(
		TEXT("mw.ShowDebug.PlayerController"),
		false,
		TEXT("Should display debug information for player controller.\n")
		TEXT("  0 = Disable (Default)\n")
		TEXT("  1 = Enable"),
		ECVF_Cheat
	);
#pragma endregion
}