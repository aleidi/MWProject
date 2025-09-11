#pragma once

// Include Header
#include "Subsystems/WorldSubsystem.h"
#include "InputActionValue.h"
#include "MWWorldSubsystem.generated.h"

// Forward Declare
class AMWInputHandler;

// Macro

/*
 * @class UMWWorldSubsystem
 * 
 * @brief
 *
 * @note
 */
UCLASS()
class UMWWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

	AMWInputHandler* GetInputHandler() const { return InputHandler; }

private:
	TObjectPtr<AMWInputHandler> InputHandler = nullptr;
};