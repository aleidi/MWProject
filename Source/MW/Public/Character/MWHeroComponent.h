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

public:
	UMWHeroComponent(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

#pragma region Input
public:
	virtual void InitializePlayerInput(UInputComponent* PlayerInputComponent);

protected:
	void Input_AbilityInputTagPressed(FGameplayTag InputTag);
	void Input_AbilityInputTagReleased(FGameplayTag InputTag);

	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_LookAt(const FInputActionValue& InputActionValue);
	void Input_LookStick(const FInputActionValue& InputActionValue);
	void Input_Crouch(const FInputActionValue& InputActionValue);
	void Input_AutoRun(const FInputActionValue& InputActionValue);
#pragma endregion

#pragma region Pawn
protected:
	void UpdatePawnRotation(float DeltaTime);

private:
	// temporary value
	FRotator LastVelocityRotation;
	FRotator DesiredPawnRotation;

	UPROPERTY(EditAnywhere, Category = "Pawn")
	float DesiredRotInterpSpeed = 460.f;

	UPROPERTY(EditAnywhere, Category = "Pawn")
	float PawnRotInterpSpeed = 10.f;

#pragma endregion
};