#include "Component/Character/MWCharacterMovementComponent.h"
#include "GameFramework/Character.h"

FVector UMWCharacterMovementComponent::GetRealVelocity() const
{
	return RealVelocity;
}

float UMWCharacterMovementComponent::GetRealSpeed() const
{
	return RealSpeed;
}

void UMWCharacterMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateRealVelocity();
}

void UMWCharacterMovementComponent::UpdateRealVelocity()
{
	if (auto* owner = GetCharacterOwner())
	{
		const FVector currPosition = owner->GetActorLocation();
		RealVelocity = currPosition - LastPosition;
		RealSpeed = RealVelocity.Size();
		LastPosition = currPosition;
	}
}