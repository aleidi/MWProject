#pragma once

#include "Gameplay/MWPawnComponent.h"
#include "MWHeroComponent.generated.h"

struct FGameplayTag;
struct FInputActionValue;
class UMWInputConfig;

UCLASS(Blueprintable, Meta=(BlueprintSpawnableComponent))
class UMWHeroComponent : public UMWPawnComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	/** Removes a mode-specific input config if it has been added */
	void RemoveAdditionalInputConfig(const UMWInputConfig* InputConfig);

	/** True if this is controlled by a real player and has progressed far enough in initialization where additional input bindings can be added */
	FORCEINLINE bool IsReadyToBindInputs() const { return bReadyToBindInputs; }
	
	virtual void InitializePlayerInput(UInputComponent* PlayerInputComponent);

protected:

	void Input_AbilityInputTagPressed(FGameplayTag InputTag);
	void Input_AbilityInputTagReleased(FGameplayTag InputTag);

	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_LookAt(const FInputActionValue& InputActionValue);
	void Input_LookStick(const FInputActionValue& InputActionValue);
	void Input_Crouch(const FInputActionValue& InputActionValue);
	void Input_AutoRun(const FInputActionValue& InputActionValue);

protected:
	//UPROPERTY(EditAnywhere)
	//TArray<FInputMappingContextAndPriority> DefaultInputMappings;

	/** True when player input bindings have been applied, will never be true for non - players */
	bool bReadyToBindInputs;
};