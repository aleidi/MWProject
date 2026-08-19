#include "Input/MWInputTriggers.h"

#include "EnhancedPlayerInput.h"
#include "InputAction.h"

ETriggerState UMWInputTriggerNotAction::UpdateState_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue ModifiedValue, float DeltaTime)
{
	if (!PlayerInput || !OtherAction)
	{
		return ETriggerState::None;
	}

	// モジュール境界を安全に越えるためActionInstanceのTrigger Eventを使用
	if (const FInputActionInstance* otherInstance = PlayerInput->FindActionInstanceData(OtherAction))
	{
		const ETriggerEvent otherEvent = otherInstance->GetTriggerEvent();

		// Trigger発生フレーム
		if (EnumHasAnyFlags(otherEvent, ETriggerEvent::Triggered))
		{
			return ETriggerState::Triggered;
		}

		// None以外のEventなら、対象Actionはこのフレームでアクティブまたは評価中
		if (otherEvent != ETriggerEvent::None)
		{
			return ETriggerState::Ongoing;
		}
	}

	return ETriggerState::None;
}
