// Copyright 2021 Alexander Shumeyko. All Rights Reserved.

#include "Common3DCameraComponent.h"

#include "Common3DCamera.h"
#include "Common3DCameraFadingObject.h"
#include "Common3DCameraMoveObject.h"
#include "Common3DCameraModeDataAsset.h"
#include "Common3DCameraVolume.h"
#include "Camera/PlayerCameraManager.h"
#include "Engine/DataTable.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"

UC3DCameraComponent::UC3DCameraComponent()
{
	MoveObjectClass = UC3DCameraMoveObject::StaticClass();
	//FOVObjectClass = UC3DCameraFOVObject::StaticClass();
	FadingObjectClass = UC3DCameraFadingObject::StaticClass();
	//FollowTerrainObjectClass = UC3DCameraFollowTerrainObject::StaticClass();
	//CameraShakesObjectClass = UC3DCameraShakesObject::StaticClass();
	//LockOnTargetObjectClass = UC3DCameraLockOnTargetObject::StaticClass();

	if (HasAnyFlags(EObjectFlags::RF_ClassDefaultObject))
	{
		RegisterConsoleCommands();
	}

	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = TG_PostPhysics;

	bAutoActivate = true;
	bTickInEditor = true;
}

void UC3DCameraComponent::OnRegister()
{
	Super::OnRegister();

	CreateCameraModesFromAssets();

	CreateAllCameraObjects();
}

void UC3DCameraComponent::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Log, TEXT("%s"), UTF8_TO_TCHAR(__FUNCTION__));

#if !UE_BUILD_SHIPPING
	CheckCameraModesErros();
#endif //!UE_BUILD_SHIPPING

	SetInitialCameraMode();
}


void UC3DCameraComponent::PostInitProperties()
{
	Super::PostInitProperties();
	UE_LOG(LogTemp, Log, TEXT("%s"), UTF8_TO_TCHAR(__FUNCTION__));
}


void UC3DCameraComponent::InitializeComponent()
{
	Super::InitializeComponent();
	UE_LOG(LogTemp, Log, TEXT("%s"), UTF8_TO_TCHAR(__FUNCTION__));
}

void UC3DCameraComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	USceneComponent::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//UE_LOG(LogTemp, Log, TEXT("%s"), UTF8_TO_TCHAR(__FUNCTION__));

	if (IsCineCameraActive())
	{
		if (!bHandledCineCamera)
		{
			bHandledCineCamera = true;

			for (auto& objPtr : CameraObjList)
			{
				objPtr->SwitchToCineCamera();
			}
		}
	}
	else
	{
		bHandledCineCamera = false;
	}

	auto cameraMode = GetCurrentCameraMode();

	//for (auto& cameraModeScript : cameraMode->CameraModeScripts)
	//{
	//	if (cameraModeScript != nullptr && cameraModeScript->IsEnableTick() && cameraModeScript->GetTickGroup() == EATPCCameraModeScriptTickGroup::PreCameraObjectTick)
	//	{
	//		cameraModeScript->Tick(DeltaTime);
	//	}
	//}

	for (auto& objPtr : CameraObjList)
	{
		objPtr->Tick(DeltaTime);
	}

	//for (auto& cameraModeScript : cameraMode->CameraModeScripts)
	//{
	//	if (cameraModeScript != nullptr && cameraModeScript->IsEnableTick() && cameraModeScript->GetTickGroup() == EATPCCameraModeScriptTickGroup::PostCameraObjectTick)
	//	{
	//		cameraModeScript->Tick(DeltaTime);
	//	}
	//}
}

void UC3DCameraComponent::ValidateComponents(bool bWithInterpolation)
{
	for (auto& cameraObj : CameraObjList)
	{
		cameraObj->Validate(bWithInterpolation);
	}
}

void UC3DCameraComponent::OnBeginOverlapCameraVolume(AC3DCameraVolume* CameraVolume)
{
	if (CameraVolume != nullptr)
	{
		SetCameraMode(CameraVolume->CameraModeTag, CameraVolume->bChangeCameraModeWithInterp);
	}
}

void UC3DCameraComponent::OnEndOverlapCameraVolume(AC3DCameraVolume* CameraVolume)
{
	AC3DCameraVolume* cameraVolume = FindOverlapCameraVolume();
	FGameplayTag newCameraModeTag = cameraVolume ? cameraVolume->CameraModeTag : DefaultCameraModeTag;

	SetCameraMode(newCameraModeTag, true);
}

void UC3DCameraComponent::SetCameraMode(FGameplayTag CameraModeTag, bool bWithInterpolation, bool bForceSet /*= false*/)
{
	if (!bForceSet && !CanSetCameraMode(CameraModeTag))
	{
		return;
	}

	if (auto cameraModePtr = SortedCameraModes.Find(CameraModeTag))
	{
		InternalSetCameraMode(*cameraModePtr, CameraModeTag, bWithInterpolation);
	}
	else
	{
		PutLog(FString::Printf(TEXT("Try set %s camera mode, but this mode not found!"), *CameraModeTag.ToString()));
	}
}

bool UC3DCameraComponent::CanSetCameraMode_Implementation(FGameplayTag CameraModeTag) const
{
	return true;
}

FGameplayTag UC3DCameraComponent::GetDesiredCameraModeTag_Implementation() const
{
	return CurrentCameraModeTag.IsValid() ? CurrentCameraModeTag : DefaultCameraModeTag;
}

FGameplayTag UC3DCameraComponent::GetInitialCameraModeTag_Implementation() const
{
	FGameplayTag cameraVolumeTag = GetCameraModeTagFromOverlapCameraVolume();
	return cameraVolumeTag.IsValid() ? cameraVolumeTag : DefaultCameraModeTag;
}

FGameplayTag UC3DCameraComponent::GetCameraModeTagFromOverlapCameraVolume() const
{
	AC3DCameraVolume* cameraVolume = FindOverlapCameraVolume();

	return cameraVolume != nullptr && cameraVolume->CameraModeTag.IsValid() ? cameraVolume->CameraModeTag : FGameplayTag();
}

FGameplayTag UC3DCameraComponent::GetCurrentCameraModeTag() const
{
	return CurrentCameraModeTag;
}

const UC3DCameraModeDataAsset* UC3DCameraComponent::GetCurrentCameraMode() const
{
	return const_cast<UC3DCameraComponent*>(this)->GetCurrentCameraMode();
}

UC3DCameraModeDataAsset* UC3DCameraComponent::GetCurrentCameraMode()
{
	UC3DCameraModeDataAsset* retValue = nullptr;

	if (CustomCameraMode != nullptr)
	{
		retValue = CustomCameraMode;
	}
	else if (auto cameraModePtr = SortedCameraModes.Find(CurrentCameraModeTag))
	{
		retValue = *cameraModePtr;
	}
	else
	{
		if (EmptyCameraMode == nullptr)
		{
			EmptyCameraMode = NewObject<UC3DCameraModeDataAsset>();
		}
		retValue = EmptyCameraMode;
	}

	return retValue;
}

void UC3DCameraComponent::SetCustomCameraMode(UC3DCameraModeDataAsset* CameraMode, bool bWithInterpolation)
{
	InternalSetCameraMode(CameraMode, {}, bWithInterpolation);
}

void UC3DCameraComponent::ResetCustomCameraMode(bool bWithInterpolation)
{
	if (IsSetCustomCameraMode())
	{
		CustomCameraMode = nullptr;
		InternalSetCameraMode(GetCurrentCameraMode(), CurrentCameraModeTag, bWithInterpolation);
	}
}

bool UC3DCameraComponent::IsSetCustomCameraMode() const
{
	return CustomCameraMode != nullptr;
}

void UC3DCameraComponent::AddCameraMode(UC3DCameraModeDataAsset* CameraMode)
{
	if (CameraMode == nullptr)
	{
		PutLog(FString::Printf(TEXT("Try AddCameraMode with null CameraMode!")));
	}
	else if (!CameraMode->ModeTag.IsValid())
	{
		PutLog(FString::Printf(TEXT("Try AddCameraMode with null CameraModeTag and CameraMode: %s!"), *GetNameSafe(CameraMode)));
	}
	else
	{
		auto instancedCameraMode = DuplicateObject(CameraMode, this);
		SortedCameraModes.Add(instancedCameraMode->ModeTag, instancedCameraMode);
	}
}

void UC3DCameraComponent::RemoveCameraMode(FGameplayTag CameraModeTag)
{
	SortedCameraModes.Remove(CameraModeTag);
}

bool UC3DCameraComponent::HasCameraMode(FGameplayTag CameraModeTag) const
{
	return SortedCameraModes.Contains(CameraModeTag);
}

UC3DCameraModeDataAsset* UC3DCameraComponent::GetCameraMode(FGameplayTag CameraModeTag) const
{
	return const_cast<UC3DCameraComponent*>(this)->GetCameraMode(CameraModeTag);
}

UC3DCameraModeDataAsset* UC3DCameraComponent::GetCameraMode(FGameplayTag CameraModeTag)
{
	auto cameraMode = SortedCameraModes.Find(CameraModeTag);
	return cameraMode != nullptr ? *cameraMode : nullptr;
}

void UC3DCameraComponent::ClearAllCameraModes()
{
	SortedCameraModes.Empty();
}

AC3DCameraVolume* UC3DCameraComponent::FindOverlapCameraVolume() const
{
	TSet<AActor*> cameraVolumes;
	GetOwner()->GetOverlappingActors(cameraVolumes, AC3DCameraVolume::StaticClass());
	return cameraVolumes.Num() > 0 ? Cast<AC3DCameraVolume>(*cameraVolumes.CreateConstIterator()) : nullptr;
}

FTransform UC3DCameraComponent::GetSocketTransform(FName InSocketName, ERelativeTransformSpace TransformSpace) const
{
	if (MoveObject != nullptr)
	{
		return MoveObject->GetSocketTransform(InSocketName, TransformSpace);
	}
	else
	{
		return Super::GetSocketTransform(InSocketName, TransformSpace);
	}
}

void UC3DCameraComponent::ZoomIn()
{
	//if (MoveObject != nullptr && GetCurrentCameraMode()->ModeSettings.bEnableLocationSettings)
	//{
	//	MoveObject->SetCameraDistance(GetCameraDistance() - GetCurrentCameraMode()->ModeSettings.LocationSettings.ZoomDistance, true);
	//}
}

void UC3DCameraComponent::ZoomOut()
{
	//if (MoveObject != nullptr && GetCurrentCameraMode()->ModeSettings.bEnableLocationSettings)
	//{
	//	MoveObject->SetCameraDistance(GetCameraDistance() + GetCurrentCameraMode()->ModeSettings.LocationSettings.ZoomDistance, true);
	//}
}

void UC3DCameraComponent::SetCameraDistance(float NewDistance, bool bInterpolate)
{
	//if (MoveObject != nullptr)
	//{
	//	MoveObject->SetCameraDistance(NewDistance, bInterpolate);
	//}
}

float UC3DCameraComponent::GetCameraDistance() const
{
	return /*MoveObject != nullptr ? MoveObject->GetCameraDistance() :*/ 0.f;
}

FVector UC3DCameraComponent::GetCameraLocation() const
{
	return GetSocketTransform(NAME_None).GetLocation();
}

FRotator UC3DCameraComponent::GetCameraRotation() const
{
	return GetSocketTransform(NAME_None).Rotator();
}

bool UC3DCameraComponent::IsCineCameraActive() const
{
	auto viewTarget = GetOwner();
	if (auto playerController = GetPlayerController())
	{
		viewTarget = playerController->GetViewTarget();
	}

	/*TArray<UCineCameraComponent*> cineCameraComponents;
	viewTarget->GetComponents(cineCameraComponents);

	return cineCameraComponents.ContainsByPredicate([](UCineCameraComponent* CineCamera) {
		return CineCamera->IsActive();
	});*/
	return false;
}

bool UC3DCameraComponent::IsSettingInitialCameraMode() const
{
	return bIsSettingInitialCameraMode;
}

APawn* UC3DCameraComponent::GetOwningPawn() const
{
	return Cast<APawn>(GetOwner());
}

APlayerController* UC3DCameraComponent::GetPlayerController() const
{
	APawn* owningPawn = GetOwningPawn();
	return owningPawn != nullptr ? Cast<APlayerController>(owningPawn->Controller) : nullptr;
}

APlayerCameraManager* UC3DCameraComponent::GetPlayerCameraManager() const
{
	APlayerController* playerController = GetPlayerController();
	return playerController != nullptr ? playerController->PlayerCameraManager : nullptr;
}

void UC3DCameraComponent::CreateCameraModesFromAssets()
{
	SortedCameraModes.Empty();
	for (auto dataAsset : CameraModesAssets)
	{
		if (dataAsset != nullptr)
		{
			AddCameraMode(dataAsset);
		}
	}
}

void UC3DCameraComponent::SetInitialCameraMode()
{
	if (!HasCameraMode(DefaultCameraModeTag))
	{
		PutLog(FString::Printf(TEXT("Default camera mode [%s] not found!"), *DefaultCameraModeTag.ToString()));
	}

	bIsSettingInitialCameraMode = true;
	SetCameraMode(GetInitialCameraModeTag(), false);
	bIsSettingInitialCameraMode = false;
}

void UC3DCameraComponent::InternalSetCameraMode(UC3DCameraModeDataAsset* NewCameraMode, TOptional<FGameplayTag> CameraModeTag, bool bWithInterpolation)
{
	if (!CameraModeTag.IsSet() || IsSetCustomCameraMode() || CurrentCameraModeTag != CameraModeTag.GetValue())
	{
		NotifyExitCameraMode();
		if (CameraModeTag.IsSet())
		{
			CustomCameraMode = nullptr;
			CurrentCameraModeTag = CameraModeTag.GetValue();
		}
		else
		{
			CustomCameraMode = NewCameraMode;
		}
		NotifyEnterCameraMode(bWithInterpolation);
		OnCameraModeChangedDelegate.Broadcast();
	}
}

void UC3DCameraComponent::NotifyExitCameraMode()
{
	for (auto& cameraObj : CameraObjList)
	{
		cameraObj->OnExitCameraMode();
	}

	//for (auto& cameraModeScript : GetCurrentCameraMode()->CameraModeScripts)
	//{
	//	if (cameraModeScript != nullptr)
	//	{
	//		cameraModeScript->OnExitCameraMode();
	//	}
	//}
}

void UC3DCameraComponent::NotifyEnterCameraMode(bool bWithInterpolation)
{
	for (auto& cameraObj : CameraObjList)
	{
		cameraObj->OnEnterCameraMode(bWithInterpolation);
	}

	//for (auto& cameraModeScript : GetCurrentCameraMode()->CameraModeScripts)
	//{
	//	if (cameraModeScript != nullptr)
	//	{
	//		cameraModeScript->OnEnterCameraMode(this);
	//	}
	//}
}

void UC3DCameraComponent::CreateAllCameraObjects()
{
	for (auto& cameraObj : CameraObjList)
	{
		if (cameraObj != nullptr)
		{
			cameraObj->ConditionalBeginDestroy();
		}
	}
	CameraObjList.Empty();

	MoveObject = CreateCameraObject(MoveObjectClass);
	//FOVObject = CreateCameraObject(FOVObjectClass);
	FadingObject = CreateCameraObject(FadingObjectClass);
	//FollowTerrainObject = CreateCameraObject(FollowTerrainObjectClass);
	//CameraShakesObject = CreateCameraObject(CameraShakesObjectClass);
	//LockOnTargetObject = CreateCameraObject(LockOnTargetObjectClass);

	for (auto objPtr : CameraObjList)
	{
		objPtr->Validate(false);
	}
}

void UC3DCameraComponent::CheckCameraModesErros()
{
	const float messageTime = 10.f;

	for (auto& cameraModeDataAsset : CameraModesAssets)
	{
		if (cameraModeDataAsset == nullptr)
		{
			continue;
		}

		if (!cameraModeDataAsset->ModeTag.IsValid())
		{
			PutLog(FString::Printf(TEXT("Discovered camera mode with invalid tag: %s"), *cameraModeDataAsset->GetName()), messageTime);
		}
		else
		{
			auto sameCameraMode = CameraModesAssets.FindByPredicate([cameraModeDataAsset](const auto& OtherCameraModeDataAsset) {
				return OtherCameraModeDataAsset != nullptr && OtherCameraModeDataAsset != cameraModeDataAsset && OtherCameraModeDataAsset->ModeTag == cameraModeDataAsset->ModeTag;
			});
			if (sameCameraMode != nullptr)
			{
				PutLog(FString::Printf(TEXT("Discovered camera modes with same tags: %s and %s. Tag: %s"), *cameraModeDataAsset->GetName(), *(*sameCameraMode)->GetName(), *cameraModeDataAsset->ModeTag.ToString()), messageTime);
			}
		}
	}
}

void UC3DCameraComponent::PutLog(const FString& Message, float MessageTime)
{
	if (DebugRules.bPrintErrorsToScreen)
	{
		GEngine->AddOnScreenDebugMessage(-1, MessageTime, FColor::Red, Message);
	}
	UE_LOG(LogC3D, Warning, TEXT("%s"), *Message);
}


void UC3DCameraComponent::RegisterConsoleCommands()
{

}

UC3DCameraBaseObject* UC3DCameraComponent::CreateCameraObjectImpl(TSubclassOf<UC3DCameraBaseObject> ObjectClass)
{
	UC3DCameraBaseObject* newObject = nullptr;
	if (ObjectClass != nullptr)
	{
		newObject = NewObject<UC3DCameraBaseObject>(this, ObjectClass);
		newObject->SetCamera(this);
		CameraObjList.Add(newObject);
	}

	return newObject;
}
