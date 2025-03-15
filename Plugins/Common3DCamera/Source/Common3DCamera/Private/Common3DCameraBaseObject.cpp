// Copyright 2021 Alexander Shumeyko. All Rights Reserved.

#include "Common3DCameraBaseObject.h"

#include "Common3DCameraComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"

#if WITH_ENGINE

UWorld* UC3DCameraBaseObject::GetWorld() const
{
	return OwningCamera != nullptr ? OwningCamera->GetWorld() : nullptr;
}

#endif //WITH_ENGINE

void UC3DCameraBaseObject::Tick(float DeltaTime)
{
	K2_Tick(DeltaTime);
}

void UC3DCameraBaseObject::Validate(bool bWithInterpolation)
{
	K2_Validate(bWithInterpolation);
}

void UC3DCameraBaseObject::OnExitCameraMode()
{
	K2_OnExitCameraMode();
}

void UC3DCameraBaseObject::OnEnterCameraMode(bool bWithInterpolation)
{
	K2_OnEnterCameraMode(bWithInterpolation);

	InitPropertyFromDataAsset();
}

void UC3DCameraBaseObject::SwitchToCineCamera()
{
}

void UC3DCameraBaseObject::SetCamera(UC3DCameraComponent* Camera)
{
	this->OwningCamera = Camera;
}

UC3DCameraComponent& UC3DCameraBaseObject::GetCamera()
{
	return *OwningCamera;
}

const UC3DCameraComponent& UC3DCameraBaseObject::GetCamera() const
{
	return *OwningCamera;
}

UC3DCameraComponent* UC3DCameraBaseObject::GetOwningCamera() const
{
	return OwningCamera;
}

AActor* UC3DCameraBaseObject::GetOwningActor() const
{
	return OwningCamera != nullptr ? OwningCamera->GetOwner() : nullptr;
}

APawn* UC3DCameraBaseObject::GetOwningPawn() const
{
	return OwningCamera != nullptr ? OwningCamera->GetOwningPawn() : nullptr;
}

APlayerController* UC3DCameraBaseObject::GetPlayerController() const
{
	return OwningCamera != nullptr ? OwningCamera->GetPlayerController() : nullptr;
}

APlayerCameraManager* UC3DCameraBaseObject::GetPlayerCameraManager() const
{
	return OwningCamera != nullptr ? OwningCamera->GetPlayerCameraManager() : nullptr;
}
