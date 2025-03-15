// Copyright 2021 Alexander Shumeyko. All Rights Reserved.

#pragma once

#include "Common3DTypes.h"
#include "Components/SceneComponent.h"
#include "GameplayTagContainer.h"

#include "Common3DCameraComponent.generated.h"

class UC3DCameraModeDataAsset;

class AC3DCameraVolume;
class UC3DCameraBaseObject;
class UC3DCameraMoveObject;
class UC3DCameraFOVObject;
class UC3DCameraFadingObject;
class UC3DCameraFollowTerrainObject;
class UC3DCameraLockOnTargetObject;
class UC3DCameraShakesObject;
class UC3DCameraModeScript;
class APawn;
class APlayerController;
class APlayerCameraManager;

/** Main component in plugin*/
UCLASS(Blueprintable, classGroup = "C3DCamera", meta = (BlueprintSpawnableComponent))
class COMMON3DCAMERA_API UC3DCameraComponent : public USceneComponent
{
	GENERATED_BODY()

	/** Called when camera mode is changed(set new camera mode, set override camera mode, reset override camera mode) */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCameraModeChangedDelegate);

public:
	UC3DCameraComponent();

	//~Begin USceneComponent Interface
	virtual FTransform GetSocketTransform(FName InSocketName, ERelativeTransformSpace TransformSpace = RTS_World) const override;
	//~End USceneComponent Interface

	//~ Begin UActorComponent Interface
	virtual void OnRegister() override;
	virtual void BeginPlay() override;
	virtual void PostInitProperties() override;
	virtual void InitializeComponent() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	//~ End UActorComponent Interface

	UFUNCTION(BlueprintCallable, Category = "C3D")
	virtual void ValidateComponents(bool bWithInterpolation);

	virtual void OnBeginOverlapCameraVolume(AC3DCameraVolume* CameraVolume);
	virtual void OnEndOverlapCameraVolume(AC3DCameraVolume* CameraVolume);

	//~ Begin Zoom Func
	UFUNCTION(BlueprintCallable, Category = "C3D|Zoom")
	void ZoomIn();
	UFUNCTION(BlueprintCallable, Category = "C3D|Zoom")
	void ZoomOut();
	UFUNCTION(BlueprintCallable, Category = "C3D|Zoom")
	void SetCameraDistance(float NewDistance, bool bInterpolate);
	//~ End Zoom Func

	/** 
	* Set camera mode with @CameraModeTag from CameraModesAssets
	* @param bForceSet - if set true, ignores CanSetCameraMode
	*/
	UFUNCTION(BlueprintCallable, Category = "C3D|CameraMode")
	virtual void SetCameraMode(FGameplayTag CameraModeTag, bool bWithInterpolation, bool bForceSet = false);

	/** This function for override */
	UFUNCTION(BlueprintPure, BlueprintNativeEvent, Category = "C3D|CameraMode")
	bool CanSetCameraMode(FGameplayTag CameraModeTag) const;

	/** Find cameraVolume and return tag if volume is valid, otherwise return @DefaultCameraModeTag */
	UFUNCTION(BlueprintPure, BlueprintNativeEvent, Category = "C3D|CameraMode")
	FGameplayTag GetInitialCameraModeTag() const;

	/** Find overlap camera volume via @FindOverlapCameraVolume and if volume and tag is valid return tag. Otherwise return invalid tag */
	UFUNCTION(BlueprintPure, Category = "C3D|CameraMode")
	FGameplayTag GetCameraModeTagFromOverlapCameraVolume() const;

	/** This function for override. By default return current tag if it is valid, otherwise return @DefaultCameraModeTag */
	UFUNCTION(BlueprintPure, BlueprintNativeEvent, Category = "C3D|CameraMode")
	FGameplayTag GetDesiredCameraModeTag() const;

	UFUNCTION(BlueprintPure, Category = "C3D|CameraMode")
	FGameplayTag GetCurrentCameraModeTag() const;

	const UC3DCameraModeDataAsset* GetCurrentCameraMode() const;
	UFUNCTION(BlueprintPure, Category = "C3D|CameraMode")
	UC3DCameraModeDataAsset* GetCurrentCameraMode();

	/** Override current camera mode(don't change current camera mode. For override used other variable) */
	UFUNCTION(BlueprintCallable, Category = "C3D|CameraMode")
	virtual void SetCustomCameraMode(UC3DCameraModeDataAsset* CameraMode, bool bWithInterpolation);
	UFUNCTION(BlueprintCallable, Category = "C3D|CameraMode")
	void ResetCustomCameraMode(bool bWithInterpolation);
	UFUNCTION(BlueprintCallable, Category = "C3D|CameraMode")
	bool IsSetCustomCameraMode() const;

	//~ Begin functions for managing camera modes
	UFUNCTION(BlueprintCallable, Category = "C3D|CameraMode")
	void AddCameraMode(UC3DCameraModeDataAsset* CameraMode);
	UFUNCTION(BlueprintCallable, Category = "C3D|CameraMode")
	void RemoveCameraMode(FGameplayTag CameraModeName);
	UFUNCTION(BlueprintPure, Category = "C3D|CameraMode")
	bool HasCameraMode(FGameplayTag CameraModeName) const;
	UC3DCameraModeDataAsset* GetCameraMode(FGameplayTag CameraModeName) const;
	UFUNCTION(BlueprintPure, Category = "C3D|CameraMode")
	UC3DCameraModeDataAsset* GetCameraMode(FGameplayTag CameraModeName);
	UFUNCTION(BlueprintCallable, Category = "C3D|CameraMode")
	void ClearAllCameraModes();
	//~ End functions for managing camera modes

	UFUNCTION(BlueprintPure, Category = "C3D")
	virtual AC3DCameraVolume* FindOverlapCameraVolume() const;

	UFUNCTION(BlueprintPure, Category = "C3D")
	float GetCameraDistance() const;

	UFUNCTION(BlueprintPure, Category = "C3D")
	FVector GetCameraLocation() const;
	UFUNCTION(BlueprintPure, Category = "C3D")
	FRotator GetCameraRotation() const;

	//UFUNCTION(BlueprintPure, Category = "C3D")
	//UC3DCameraMoveObject* GetCameraLocationObject() const { return MoveObject; }
	//UFUNCTION(BlueprintPure, Category = "C3D")
	//UC3DCameraFOVObject* GetCameraFOVObject() const { return FOVObject; }
	//UFUNCTION(BlueprintPure, Category = "C3D")
	//UC3DCameraFadingObject* GetCameraFadingObject() const { return FadingObject; }
	//UFUNCTION(BlueprintPure, Category = "C3D")
	//UC3DCameraFollowTerrainObject* GetCameraFollowTerrainObject() const { return FollowTerrainObject; }
	//UFUNCTION(BlueprintPure, Category = "C3D")
	//UC3DCameraShakesObject* GetCameraShakesObject() const { return CameraShakesObject; }
	//UFUNCTION(BlueprintPure, Category = "C3D")
	//UC3DCameraLockOnTargetObject* GetCameraLockOnTargetObject() const { return LockOnTargetObject; }

	UFUNCTION(BlueprintPure, Category = "C3D")
	virtual bool IsCineCameraActive() const;

	/** Return true if initial camera mode is sets now(need for correct initialization some camera objects)*/
	bool IsSettingInitialCameraMode() const;

	template<typename T>
	T* CreateCameraObject(TSubclassOf<T> ObjectClass)
	{
		static_assert(TPointerIsConvertibleFromTo<T, const UC3DCameraBaseObject>::Value, "'T' template parameter to CreateCameraObject must be derived from UC3DCameraBaseObject");
		UClass* objClass = ObjectClass != nullptr ? ObjectClass.Get() : T::StaticClass();
		return CastChecked<T>(CreateCameraObjectImpl(objClass));
	}

	UFUNCTION(BlueprintPure, Category = "C3D")
	APawn* GetOwningPawn() const;
	UFUNCTION(BlueprintPure, Category = "C3D")
	APlayerController* GetPlayerController() const;
	UFUNCTION(BlueprintPure, Category = "C3D")
	APlayerCameraManager* GetPlayerCameraManager() const;

public:
	UPROPERTY(BlueprintAssignable, Category = "C3D")
	FOnCameraModeChangedDelegate OnCameraModeChangedDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C3D")
	FC3DCameraDebugRules DebugRules;

protected:
	/* Create camera modes from CameraModeAssets */
	virtual void CreateCameraModesFromAssets();

	virtual void SetInitialCameraMode();

	virtual void InternalSetCameraMode(UC3DCameraModeDataAsset* NewCameraMode, TOptional<FGameplayTag> CameraModeTag, bool bWithInterpolation);

	virtual void NotifyExitCameraMode();
	virtual void NotifyEnterCameraMode(bool bWithInterpolation);

	virtual void CreateAllCameraObjects();

	void CheckCameraModesErros();
	void PutLog(const FString& Message, float MessageTime = 5.f);

	void RegisterConsoleCommands();

protected:
	UC3DCameraBaseObject* CreateCameraObjectImpl(TSubclassOf<UC3DCameraBaseObject> ObjectClass);

protected:
	bool bIsSettingInitialCameraMode = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C3D|Settings")
	FGameplayTag DefaultCameraModeTag;

	/** Data assets with camera modes */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C3D|Settings")
	TArray<UC3DCameraModeDataAsset*> CameraModesAssets;

	/** Storage for instanced camera modes sorted by FGameplayTag */
	UPROPERTY()
	TMap<FGameplayTag, UC3DCameraModeDataAsset*> SortedCameraModes;

	UPROPERTY()
	UC3DCameraModeDataAsset* CustomCameraMode;

	FGameplayTag CurrentCameraModeTag;

	UPROPERTY(EditAnywhere, NoClear, Category = "C3D|Settings|CameraObjects")
	TSubclassOf<UC3DCameraMoveObject> MoveObjectClass;
	//UPROPERTY(EditAnywhere, NoClear, Category = "C3D|Settings|CameraObjects")
	//TSubclassOf<UC3DCameraFOVObject> FOVObjectClass;
	UPROPERTY(EditAnywhere, NoClear, Category = "C3D|Settings|CameraObjects")
	TSubclassOf<UC3DCameraFadingObject> FadingObjectClass;
	//UPROPERTY(EditAnywhere, NoClear, Category = "C3D|Settings|CameraObjects")
	//TSubclassOf<UC3DCameraFollowTerrainObject> FollowTerrainObjectClass;
	//UPROPERTY(EditAnywhere, NoClear, Category = "C3D|Settings|CameraObjects")
	//TSubclassOf<UC3DCameraShakesObject> CameraShakesObjectClass;
	//UPROPERTY(EditAnywhere, NoClear, Category = "C3D|Settings|CameraObjects")
	//TSubclassOf<UC3DCameraLockOnTargetObject> LockOnTargetObjectClass;

	UPROPERTY(Transient)
	UC3DCameraMoveObject* MoveObject;
	//UPROPERTY(Transient)
	//UC3DCameraFOVObject* FOVObject;
	UPROPERTY(Transient)
	UC3DCameraFadingObject* FadingObject;
	//UPROPERTY(Transient)
	//UC3DCameraFollowTerrainObject* FollowTerrainObject;
	//UPROPERTY(Transient)
	//UC3DCameraShakesObject* CameraShakesObject;
	//UPROPERTY(Transient)
	//UC3DCameraLockOnTargetObject* LockOnTargetObject;

	UPROPERTY(Transient)
	TArray<UC3DCameraBaseObject*> CameraObjList;

	//Handled cine camera after switch target view?
	bool bHandledCineCamera = false;

private:
	UPROPERTY()
	UC3DCameraModeDataAsset* EmptyCameraMode;
	/* For correct view FATPCCameraMode in Blueprints Details panel */
	UPROPERTY()
	FC3DCameraMode CameraModeDEV;
};
