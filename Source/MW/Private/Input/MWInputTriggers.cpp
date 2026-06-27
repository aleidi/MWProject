#include "Input/MWInputTriggers.h"

#include "EnhancedPlayerInput.h"
#include "InputAction.h"

ETriggerState UMWInputTriggerNotAction::UpdateState_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue ModifiedValue, float DeltaTime)
{
	if (!PlayerInput || !OtherAction)
	{
		return ETriggerState::None;
	}

	// Use trigger event from ActionInstance (link-safe across module boundary)
	if (const FInputActionInstance* otherInstance = PlayerInput->FindActionInstanceData(OtherAction))
	{
		const ETriggerEvent otherEvent = otherInstance->GetTriggerEvent();

		// Triggered frame
		if (EnumHasAnyFlags(otherEvent, ETriggerEvent::Triggered))
		{
			return ETriggerState::Triggered;
		}

		// Any non-none event means the other action is active/evaluating this frame
		if (otherEvent != ETriggerEvent::None)
		{
			return ETriggerState::Ongoing;
		}
	}

	return ETriggerState::None;
}
