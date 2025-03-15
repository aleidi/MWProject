// Copyright 2021 Alexander Shumeyko. All Rights Reserved.

#pragma once

#include "Common3DCameraBaseObject.h"
#include "Engine/EngineTypes.h"

#include "Common3DCameraFadingObject.generated.h"

class AActor;

struct FC3DFadingActorInfo
{
public:
	FC3DFadingActorInfo() = default;
	FC3DFadingActorInfo(AActor* InActor, bool bInFadeOut, float InFadeOutTime, float InFadeValue = 1.f);

	bool operator==(const FC3DFadingActorInfo& other) const;

public:
	TWeakObjectPtr<AActor> Actor;
	// bFadeOut is Disappear
	bool bFadeOut;
	float FadeOutTime;
	float FadeValue;
};

UCLASS(classGroup = "C3DCamera")
class COMMON3DCAMERA_API UC3DCameraFadingObject : public UC3DCameraBaseObject
{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaTime) override;

	virtual void Validate(bool bWithInterpolation) override;
	virtual void OnEnterCameraMode(bool bWithInterpolation) override;

	UFUNCTION(BlueprintCallable, Category = "C3D")
	void AddIgnoredActor(AActor* Actor);
	UFUNCTION(BlueprintCallable, Category = "C3D")
	void RemoveIgnoredActor(AActor* Actor);
	UFUNCTION(BlueprintCallable, Category = "C3D")
	void ClearIgnoredActors();

	UFUNCTION(BlueprintCallable, Category = "C3D")
	void AddManualFadeActor(AActor* Actor, bool bWithCustomTime = false, float CustomFadeOutTime = 0.f);
	UFUNCTION(BlueprintCallable, Category = "C3D")
	void RemoveManualFadeActor(AActor* Actor);
	UFUNCTION(BlueprintCallable, Category = "C3D")
	void ClearManualFadeActors();

protected:
	virtual void SelfFade();

	// Trace and find collided actors
	virtual TArray<FHitResult> FindCollidedActors() const;
	virtual TArray<AActor*> FilterCollidedActors(const TArray<FHitResult>& HitResults);

	virtual void UpdateFadingActorsList(TArray<AActor*> RelevantActors);
	virtual void ApplyFade(float DeltaTime);

	virtual void InitPropertyFromDataAsset() override;

protected:
	TArray<FC3DFadingActorInfo> FadingActors;

	TSet<TWeakObjectPtr<AActor>> IgnoredActors;
	TSet<TWeakObjectPtr<AActor>> ManualFadingActors;

private:
	/** fading setting property */
	bool bEnableFade = true;

	float FadeInTime = 0.8f;
	float FadeOutTime = 0.8f;

	/** Material param. During fade process this. Not support changing between different camera modes */

	float MaterialFadeMinValue = 0.f;
	float MaterialFadeMaxValue = 1.f;

	/** 
	* The set of material parameters that will be set to the minimum @MaterialFadeMinValue or maximum @MaterialFadeMaxValue value.
	* These parameters (usually only 1 here) should hide or show the object.
	*/
	TArray<FName> MaterialFadeParamNames;

	/** 
	* This channel used in trace for find for objects are between camera and character.
	* see @UC3DCameraFadingObject::FindCollidedActors
	*/
	ECollisionChannel FadeChannel = ECC_Camera;

	/** 
	* Do need hide self character if he collided with camera.
	* For camera check radius see @SelfFadeCheckRadius
	*/
	bool bFadeSelfIfCollision = true;

	/** If true, then custom fade time will be used @SelfFadeCustomFadeOutTime, otherwise will be used @FadeOutTime*/
	bool bUseCustomFadeOutTimeForSelfFade = false;

	/** Custom fade time if @bUseCustomFadeOutTimeForSelfFade is true. Overrides @FadeOutTime */
	float SelfFadeCustomFadeOutTime = 1.f;

	/** Radius sphere trace to determine camera collision with character*/
	float SelfFadeCheckRadius = 18.f;

	/** If true, and if bFadeSelfIfCollision true then all attached actors to character will be faded */
	bool bSelfFadeAttachedActors = true;
};