// Copyright 2021 Alexander Shumeyko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"

#include "Common3DTypes.generated.h"

class UCurveVector;
class UCurveFloat;

USTRUCT(BlueprintType)
struct COMMON3DCAMERA_API FC3DMoveSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move|Rotation")
	uint8 bUsePawnControlRotation : 1 = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move|Rotation")
	uint8 bInheritPitch : 1 = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move|Rotation")
	uint8 bInheritYaw : 1 = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move|Rotation")
	uint8 bInheritRoll : 1 = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move|SpringArm")
	float TargetArmLength = 300.f;

	/* The offset from start of spring arm to the target pivot location. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move|SpringArm")
	FVector TargetPivotOffset = FVector::ZeroVector;

	/* The offset from end of spring arm to the camera location. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move|SpringArm")
	FVector CameraOffset = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move|SpringArm")
	bool bEnableRotLag = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move|SpringArm", meta = (EditCondition = "ShouldEnableRotLag"))
	float RotLagSpeed = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move|SpringArm")
	bool bEnableLocLag = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move|SpringArm", meta = (EditCondition = "ShouldEnableLocLag"))
	float LocLagSpeed = 10.f;

	/** If true, do a collision test using ProbeChannel and ProbeSize to prevent camera clipping into level. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Move|Collision")
	uint8 bCollisionTest : 1 = true;

	/** Collision channel of the query probe (defaults to ECC_Camera) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move|Collision", meta = (editcondition = "ShouldDoCollisionTest"))
	TEnumAsByte<ECollisionChannel> ProbeChannel = ECollisionChannel::ECC_Camera;

	/** How big should the query probe sphere be (in unreal units) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Move|Collision", meta=(editcondition = "ShouldDoCollisionTest"))
	float ProbeSize = 12.0f;
};

USTRUCT(BlueprintType)
struct COMMON3DCAMERA_API FC3DFadeSettings
{
	GENERATED_BODY()

	/**
	* Time to completely show the object (if the material parameter = MaterialFadeMaxValue)
	*
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fade", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float FadeInTime = 0.8f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fade", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float FadeOutTime = 0.8f;

	/** Material param. During fade process this. Not support changing between different camera modes */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fade")
	float MaterialFadeMinValue = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fade")
	float MaterialFadeMaxValue = 1.f;

	/** 
	* The set of material parameters that will be set to the minimum @MaterialFadeMinValue or maximum @MaterialFadeMaxValue value.
	* These parameters (usually only 1 here) should hide or show the object.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fade")
	TArray<FName> MaterialFadeParamNames;

	/** 
	* This channel used in trace for find for objects are between camera and character.
	* see @UC3DCameraFadingObject::FindCollidedActors
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fade")
	TEnumAsByte<ECollisionChannel> FadeChannel = ECC_Camera;

	/** 
	* Do need hide self character if he collided with camera.
	* For camera check radius see @SelfFadeCheckRadius
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fade")
	bool bFadeSelfIfCollision = true;

	/** If true, then custom fade time will be used @SelfFadeCustomFadeOutTime, otherwise will be used @FadeOutTime*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fade", meta = (InlineEditConditionToggle))
	bool bUseCustomFadeOutTimeForSelfFade = false;

	/** Custom fade time if @bUseCustomFadeOutTimeForSelfFade is true. Overrides @FadeOutTime */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fade", meta = (EditCondition = "bUseCustomFadeOutTimeForSelfFade", ClampMin = "0.0", UIMin = "0.0"))
	float SelfFadeCustomFadeOutTime = 1.f;

	/** Radius sphere trace to determine camera collision with character*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fade", meta = (EditCondition = "bFadeSelfIfCollision", ClampMin = "1.0", UIMin = "1.0"))
	float SelfFadeCheckRadius = 18.f;

	/** If true, and if bFadeSelfIfCollision true then all attached actors to character will be faded */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fade", meta = (EditCondition = "bFadeSelfIfCollision"))
	bool bSelfFadeAttachedActors = true;
};

USTRUCT(BlueprintType)
struct COMMON3DCAMERA_API FC3DCameraMode
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (InlineEditConditionToggle))
	bool bEnableMoveSettings = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (EditCondition = "bEnableMoveSettings"))
	FC3DMoveSettings MoveSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (InlineEditConditionToggle))
	bool bEnableFadeSettings = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (EditCondition = "bEnableFadeSettings"))
	FC3DFadeSettings FadeSettings;
};

USTRUCT(BlueprintType)
struct COMMON3DCAMERA_API FC3DCameraDebugRules
{
	GENERATED_BODY()

	/** Put errors to log and screen, otherwise put only to log */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Logs")
	bool bPrintErrorsToScreen = true;

	/**
	* Debug object - CameraLocationObject
	* Show debug information about location object
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	bool bEnableMoveObjectDebug = false;

	/**
	* Debug object - CameraFadingObject
	* Show debug trace for find objects between camera and character
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	bool bEnableDrawFadeShapeDebug = false;
};
