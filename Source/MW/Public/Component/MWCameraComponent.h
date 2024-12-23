#pragma once

#include "CoreMinimal.h"

class FMWCameraComponent : public TSharedFromThis<FMWCameraComponent> 
{
public:
	FMWCameraComponent() = delete;
	FMWCameraComponent(AActor* Owner);

private:
	/** Camera boom positioning the camera behind the character */
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	class UCameraComponent* FollowCamera;
};