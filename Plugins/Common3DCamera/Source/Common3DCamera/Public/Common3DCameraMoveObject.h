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
	 * ブーム回転の基準となる、継承対象の回転を取得する。
	 * 親へのアタッチ状態、UsePawnControlRotation、絶対回転フラグから算出する。
	 */
	FRotator GetTargetRotation() const;

	/** コリジョンテストによる変位を適用しないカメラ位置を取得する */
	FVector GetUnfixedCameraPosition() const;

	/** コリジョンテストによる変位が適用されているか */
	bool IsCollisionFixApplied() const;

	/** bInheritPitchなどの回転制約を適用する前のスプリングアームの目標回転を返す */
	virtual FRotator GetDesiredRotation() const;

	/** Pawnからビュー回転を取得する。 */
	FRotator GetViewRotation() const;

protected:
	virtual void UpdateDesiredArmLocation(float DeltaTime);

	virtual FVector BlendLocations(const FVector& DesiredArmLocation, const FVector& TraceHitLocation, bool bHitSomething, float DeltaTime);

	virtual void InitPropertyFromDataAsset() override;

	virtual void ShowDebug();
	void AddHitResultToDebug(const FHitResult& HitResult);

protected:
	/* 状態キャッシュ用の一時変数。 */
	FRotator LastDesiredRot = FRotator::ZeroRotator;
	FVector LastDesiredLoc = FVector::ZeroVector;

	FVector UnfixedCameraPosition = FVector::ZeroVector;
	FVector RelativeSocketLocation = FVector::ZeroVector;
	FQuat RelativeSocketRotation = FQuat::Identity;

	uint8 bIsCameraFixed : 1 = false;

	// カメラ設定プロパティ
	uint8 bEnableMoveSettings : 1 = false;

	uint8 bUsePawnControlRotation : 1 = true;

	uint8 bInheritPitch : 1 = true;

	uint8 bInheritYaw : 1 = true;

	uint8 bInheritRoll : 1 = true;

	FVector CameraOffset = FVector::ZeroVector;

	FRotator CameraRotationOffset = FRotator::ZeroRotator;

	FVector TargetPivotOffset = FVector::ZeroVector;

	float TargetArmLength = 300.f;

	/** 位置遅延設定 */
	bool bEnableLocLag = false;

	float LocLagSpeed = 10.f;

	/** 回転遅延設定 */
	bool bEnableRotLag = true;

	float RotLagSpeed = 10.f;

	/** コリジョンテスト */
	uint8 bCollisionTest : 1 = true;

	ECollisionChannel ProbeChannel = ECollisionChannel::ECC_Camera;

	float ProbeSize = 12.0f;

	/** デバッグデータ */
	FDebugData DebugData;
};
