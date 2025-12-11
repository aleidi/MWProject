#pragma once

#include "CoreMinimal.h"

class FMWBattleCameraControl : public TSharedFromThis<FMWBattleCameraControl>
{
	friend class UMWCommandBattle;

public:
	void SetCameraOnTurnBegin();
};