#pragma once

#include "GameFramework/CharacterMovementComponent.h"
#include "MWCharacterMovementComponent.generated.h"

UCLASS()
class UMWCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="Movement")
	FVector GetRealVelocity() const;
	UFUNCTION(BlueprintPure, Category="Movement")
	float GetRealSpeed() const;

protected:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void UpdateRealVelocity();
private:
	UPROPERTY()
	FVector RealVelocity;
	UPROPERTY()
	float RealSpeed;
	UPROPERTY()
	FVector LastPosition;
};