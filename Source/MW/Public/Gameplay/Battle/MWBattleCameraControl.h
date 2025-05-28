#pragma once

#include "CoreMinimal.h"

class FMWBattleCameraControl : public TSharedFromThis<FMWBattleCameraControl>
{
	friend class UMWBattle;

public:
	void SetCameraOnTurnBegin();
};