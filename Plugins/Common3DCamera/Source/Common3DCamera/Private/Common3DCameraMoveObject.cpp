#include "Common3DCameraMoveObject.h"
#include "Common3DCameraComponent.h"
#include "Common3DCameraModeDataAsset.h"

void UC3DCameraMoveObject::Tick(float DeltaTime)
{
	UpdateDesiredArmLocation(DeltaTime);

#if !UE_BUILD_SHIPPING
	if (GetCamera().DebugRules.bEnableMoveObjectDebug)
	{
		ShowDebug();
	}
#endif
}

void UC3DCameraMoveObject::OnExitCameraMode()
{

}

void UC3DCameraMoveObject::OnEnterCameraMode(bool bWithInterpolation)
{
	Super::OnEnterCameraMode(bWithInterpolation);
}

void UC3DCameraMoveObject::UpdateDesiredArmLocation(float DeltaTime)
{
	auto& camera = GetCamera();

	// 回転を計算
	FRotator desiredRot = GetTargetRotation();
	if (bEnableRotLag)
	{
		desiredRot = FRotator(FMath::QInterpTo(LastDesiredRot.Quaternion(), desiredRot.Quaternion(), DeltaTime, RotLagSpeed));
	}
	LastDesiredRot = desiredRot;

	// 位置を計算
	const FVector targetOrigin = camera.GetComponentLocation();
	FVector desiredLoc = targetOrigin + TargetPivotOffset;

	if (bEnableLocLag)
	{
		desiredLoc = FMath::VInterpTo(LastDesiredLoc, desiredLoc, DeltaTime, LocLagSpeed);
	}
	LastDesiredLoc = desiredLoc;

	// 回転方向に沿ってカメラ位置を後方へオフセット
	desiredLoc -= desiredRot.Vector() * TargetArmLength;
	// ローカル空間でソケットオフセットを加算
	desiredLoc += FRotationMatrix(desiredRot).TransformVector(CameraOffset);

	// カメラが他のオブジェクトを貫通しないようスイープを実行
	FVector resultLoc;
	if (bCollisionTest)
	{
		bIsCameraFixed = true;
		FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(SpringArm), false, camera.GetOwner());

		FHitResult result;
		GetWorld()->SweepSingleByChannel(result, targetOrigin, desiredLoc, FQuat::Identity, ProbeChannel, FCollisionShape::MakeSphere(ProbeSize), QueryParams);

		resultLoc = BlendLocations(desiredLoc, result.Location, result.bBlockingHit, DeltaTime);
		UnfixedCameraPosition = desiredLoc;

		if (resultLoc == desiredLoc)
		{
			bIsCameraFixed = false;
		}
	}
	else
	{
		resultLoc = desiredLoc;
		bIsCameraFixed = false;
		UnfixedCameraPosition = resultLoc;
	}

	// カメラの新しいワールドトランスフォームを生成
	FTransform worldCamTM(desiredRot, resultLoc);
	// コンポーネント相対へ変換
	FTransform relCamTM = worldCamTM.GetRelativeTransform(camera.GetComponentTransform());

	// ソケットの位置と回転を更新
	RelativeSocketLocation = relCamTM.GetLocation();
	RelativeSocketRotation = relCamTM.GetRotation();

	camera.UpdateChildTransforms();
}

FTransform UC3DCameraMoveObject::GetSocketTransform(FName InSocketName, ERelativeTransformSpace TransformSpace) const
{
	auto& camera = GetCamera();

	FTransform socketTransoform = FTransform::Identity;

	FRotator cameraRotation = RelativeSocketRotation.Rotator().GetNormalized();

	FTransform relativeTransform(cameraRotation.Quaternion(), RelativeSocketLocation);

	switch (TransformSpace)
	{
		case RTS_World:
		{
			socketTransoform = relativeTransform * camera.GetComponentTransform();
			break;
		}
		case RTS_Actor:
		{
			if (const AActor* Actor = camera.GetOwner())
			{
				socketTransoform = relativeTransform * camera.GetComponentTransform();
				socketTransoform = socketTransoform.GetRelativeTransform(Actor->GetTransform());
			}
			break;
		}
		case RTS_Component:
		{
			socketTransoform = relativeTransform;
			break;
		}
	}

	return socketTransoform;
}

FRotator UC3DCameraMoveObject::GetTargetRotation() const
{
	auto& camera = GetCamera();
	FRotator desiredRot = bUsePawnControlRotation ? GetViewRotation() : GetDesiredRotation();

	// 回転を継承する場合、各軸の継承設定を確認
	if (!camera.IsUsingAbsoluteRotation())
	{
		const FRotator LocalRelativeRotation = camera.GetRelativeRotation();
		if (!bInheritPitch)
		{
			desiredRot.Pitch = LocalRelativeRotation.Pitch;
		}

		if (!bInheritYaw)
		{
			desiredRot.Yaw = LocalRelativeRotation.Yaw;
		}

		if (!bInheritRoll)
		{
			desiredRot.Roll = LocalRelativeRotation.Roll;
		}
	}

	return desiredRot + CameraRotationOffset;
}

FVector UC3DCameraMoveObject::GetUnfixedCameraPosition() const
{
	return UnfixedCameraPosition;
}

bool UC3DCameraMoveObject::IsCollisionFixApplied() const
{
	return bIsCameraFixed;
}

FRotator UC3DCameraMoveObject::GetDesiredRotation() const
{
	return GetCamera().GetComponentRotation();
}

FRotator UC3DCameraMoveObject::GetViewRotation() const
{
	APawn* pawn = GetOwningPawn();
	return pawn != nullptr ? pawn->GetViewRotation() : GetDesiredRotation();
}

FVector UC3DCameraMoveObject::BlendLocations(const FVector& DesiredArmLocation, const FVector& TraceHitLocation, bool bHitSomething, float DeltaTime)
{
	return bHitSomething ? TraceHitLocation : DesiredArmLocation;
}

void UC3DCameraMoveObject::InitPropertyFromDataAsset()
{
	const auto& camera = GetCamera();
	const auto& modeSetting = camera.GetCurrentCameraMode()->ModeSettings;
	const auto& moveSettings = modeSetting.MoveSettings;

	bEnableMoveSettings = modeSetting.bEnableMoveSettings;

	bUsePawnControlRotation = moveSettings.bUsePawnControlRotation;

	bInheritPitch = moveSettings.bInheritPitch;

	bInheritYaw = moveSettings.bInheritYaw;

	bInheritRoll = moveSettings.bInheritRoll;

	CameraOffset = moveSettings.CameraOffset;

	CameraRotationOffset = moveSettings.CameraRotationOffset;

	TargetPivotOffset = moveSettings.TargetPivotOffset;

	TargetArmLength = moveSettings.TargetArmLength;

	bEnableLocLag = bEnableMoveSettings ? moveSettings.bEnableLocLag : false;

	LocLagSpeed = moveSettings.LocLagSpeed;

	bEnableRotLag = bEnableMoveSettings ? moveSettings.bEnableRotLag : false;

	RotLagSpeed = moveSettings.RotLagSpeed;

	bCollisionTest = moveSettings.bCollisionTest;

	ProbeChannel = moveSettings.ProbeChannel;

	ProbeSize = moveSettings.ProbeSize;
}

void UC3DCameraMoveObject::ShowDebug()
{
	const FColor color = FColor::Green;
	GEngine->AddOnScreenDebugMessage(-1, 0.f, color, TEXT("----- Camera Location Object End Debug -----"));

	GEngine->AddOnScreenDebugMessage(-1, 0.f, color, FString::Printf(TEXT("Trace hit component: %s"), *GetNameSafe(DebugData.TraceHitResult.GetComponent())));
	GEngine->AddOnScreenDebugMessage(-1, 0.f, color, FString::Printf(TEXT("Trace hit actor: %s"), *GetNameSafe(DebugData.TraceHitResult.GetActor())));

	GEngine->AddOnScreenDebugMessage(-1, 0.f, color, FString::Printf(TEXT("Trace blocking hit: %s"), *LexToString(DebugData.TraceHitResult.bBlockingHit)));
	//GEngine->AddOnScreenDebugMessage(-1, 0.f, color, FString::Printf(TEXT("Is blend distance: %s"), *LexToString(IsBlendingDistanceAfterFixedLocation())));

	//GEngine->AddOnScreenDebugMessage(-1, 0.f, color, FString::Printf(TEXT("Current target distance: %.2f"), CameraDistance.TargetDistance));
	//GEngine->AddOnScreenDebugMessage(-1, 0.f, color, FString::Printf(TEXT("Current camera distance: %.2f"), CameraDistance.CurrentDistance));
	//GEngine->AddOnScreenDebugMessage(-1, 0.f, color, FString::Printf(TEXT("Real camera distance: %.2f"), GetRealCameraDistance()));

	GEngine->AddOnScreenDebugMessage(-1, 0.f, color, TEXT("----- Camera Location Object Start Debug -----"));
}

void UC3DCameraMoveObject::AddHitResultToDebug(const FHitResult& HitResult)
{
#if !UE_BUILD_SHIPPING
	if (GetCamera().DebugRules.bEnableMoveObjectDebug)
	{
		DebugData.TraceHitResult = HitResult;
	}
#endif
}
