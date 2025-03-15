// Copyright 2021 Alexander Shumeyko. All Rights Reserved.

#pragma once

#include "Common3DCameraBaseObject.h"
#include "Common3DTypes.h"
#include "Engine/EngineTypes.h"
#include "GameplayTagContainer.h"

#include "Common3DCameraMoveObject.generated.h"

enum ERelativeTransformSpace;

UCLASS(classGroup = "C3DCamera")
class COMMON3DCAMERA_API UC3DCameraMoveObject : public UC3DCameraBaseObject
{
	GENERATED_BODY()

private:
	struct FDebugData
	{
		FHitResult TraceHitResult;
	};

public:
	virtual void Tick(float DeltaTime) override;
	virtual void OnExitCameraMode() override;
	virtual void OnEnterCameraMode(bool bWithInterpolation) override;

	virtual FTransform GetSocketTransform(FName InSocketName, ERelativeTransformSpace TransformSpace) const;

private:
	/**
	 * Get the target rotation we inherit, used as the base target for the boom rotation.
	 * This is derived from attachment to our parent and considering the UsePawnControlRotation and absolute rotation flags.
	 */
	FRotator GetTargetRotation() const;

	/** Get the position where the camera should be without applying the Collision Test displacement */
	FVector GetUnfixedCameraPosition() const;

	/** Is the Collision Test displacement being applied? */
	bool IsCollisionFixApplied() const;

	/** Returns the desired rotation for the spring arm, before the rotation constraints such as bInheritPitch etc are enforced. */
	virtual FRotator GetDesiredRotation() const;

	/** Get view rotation from pawn. */
	FRotator GetViewRotation() const;

protected:
	virtual void UpdateDesiredArmLocation(float DeltaTime);

	virtual FVector BlendLocations(const FVector& DesiredArmLocation, const FVector& TraceHitLocation, bool bHitSomething, float DeltaTime);

	virtual void InitPropertyFromDataAsset() override;

	virtual void ShowDebug();
	void AddHitResultToDebug(const FHitResult& HitResult);

protected:
	/* Temporary variable for cache state. */
	FRotator LastDesiredRot = FRotator::ZeroRotator;
	FVector LastDesiredLoc = FVector::ZeroVector;

	FVector UnfixedCameraPosition = FVector::ZeroVector;
	FVector RelativeSocketLocation = FVector::ZeroVector;
	FQuat RelativeSocketRotation = FQuat::Identity;

	uint8 bIsCameraFixed : 1 = false;

	// Camera Setting Property
	uint8 bEnableMoveSettings : 1 = false;

	uint8 bUsePawnControlRotation : 1 = true;

	uint8 bInheritPitch : 1 = true;

	uint8 bInheritYaw : 1 = true;

	uint8 bInheritRoll : 1 = true;

	FVector CameraOffset = FVector::ZeroVector;

	FVector TargetPivotOffset = FVector::ZeroVector;

	float TargetArmLength = 300.f;

	/** location lag setting */
	bool bEnableLocLag = false;

	float LocLagSpeed = 10.f;

	/** rotation lag setting */
	bool bEnableRotLag = true;

	float RotLagSpeed = 10.f;

	/** collision test */
	uint8 bCollisionTest : 1 = true;

	ECollisionChannel ProbeChannel = ECollisionChannel::ECC_Camera;

	float ProbeSize = 12.0f;

	/** Debug Data */
	FDebugData DebugData;
};