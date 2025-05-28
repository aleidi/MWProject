#pragma once

#include "Gameplay/MWPawnComponent.h"
#include "MWCharacterBattleComponent.generated.h"

UCLASS(Blueprintable, Meta=(BlueprintSpawnableComponent))
class UMWCharacterBattleComponent : public UMWPawnComponent
{
	GENERATED_BODY()

public:
	/* Play animation according to command. When the animation is finished, OnActionComplete will be broadcast. */
	void PlayActionAnim();

	DECLARE_DELEGATE(FOnActionComplete);
	FOnActionComplete& OnActionComplete();

private:
	FOnActionComplete OnActionCompleteDele;
};