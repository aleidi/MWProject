#pragma once

// Include
#include "CoreMinimal.h"
#include "Gameplay/Battle/MWBattle.h"

// Forward Declare

// Macro

/**
 * @class FMWBattleManager
 * 
 * @brief Camera, UI, Character and any other logic used in Battle can put in this class.
 *		  
 * @note
**/
class FMWBattleManager : public TSharedFromThis<FMWBattleManager>
{
public:
	FMWBattleManager() = default;
	~FMWBattleManager() = default;

	/* Use character's camera as the main camera.
	*  The character is the leader of the team.
	*/
	void SetCharacterCameraAsMain(UMWBattle& Context);

	void EnableBattleCommandUI(UMWBattle& Context);
};