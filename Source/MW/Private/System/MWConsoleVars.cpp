#include "System/MWConsoleVars.h"

namespace MWConsoleVars
{
	// ==== Ability / Skill ====
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

	// ==== Input ====
#pragma region Input
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