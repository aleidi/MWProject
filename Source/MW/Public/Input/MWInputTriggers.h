#pragma once

// Include Header
#include "InputTriggers.h"
#include "MWInputTriggers.generated.h"

// Forward Declare

// Define

/*
 * @class UMWInputTriggerNotAction
 * 
 * @brief 
 *
 * @note
 */
UCLASS(NotBlueprintable, meta = (DisplayName = "Trigger Not Action", NotInputConfigurable = "true"))
class UMWInputTriggerNotAction : public UInputTrigger
{
	GENERATED_BODY()
	
protected:
	virtual ETriggerType GetTriggerType_Implementation() const override
	{
		return ETriggerType::Blocker;
	}

	virtual ETriggerState UpdateState_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue ModifiedValue, float DeltaTime) override;

	virtual bool IsBlocking(const ETriggerState State) const override
	{
		// 只要 OtherAction 有状态，就阻断当前 Action
		return State != ETriggerState::None;
	}

public:
	UPROPERTY(EditAnywhere, Category = "Trigger Settings")
	TObjectPtr<const UInputAction> OtherAction = nullptr;
};
