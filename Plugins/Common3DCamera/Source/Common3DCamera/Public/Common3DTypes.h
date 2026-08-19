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

	/* スプリングアームの始点からターゲットピボット位置までのオフセット。 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move|SpringArm")
	FVector TargetPivotOffset = FVector::ZeroVector;

	/* スプリングアームの終点からカメラ位置までのオフセット。 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move|SpringArm")
	FVector CameraOffset = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move|SpringArm")
	FRotator CameraRotationOffset = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move|SpringArm")
	bool bEnableRotLag = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move|SpringArm", meta = (EditCondition = "bEnableRotLag"))
	float RotLagSpeed = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move|SpringArm")
	bool bEnableLocLag = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move|SpringArm", meta = (EditCondition = "bEnableLocLag"))
	float LocLagSpeed = 10.f;

	/** trueの場合、ProbeChannelとProbeSizeを使用してコリジョンテストを行い、カメラのレベルへのめり込みを防ぐ。 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Move|Collision")
	uint8 bCollisionTest : 1 = true;

	/** クエリプローブのコリジョンチャンネル（デフォルトはECC_Camera） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move|Collision", meta = (editcondition = "bCollisionTest"))
	TEnumAsByte<ECollisionChannel> ProbeChannel = ECollisionChannel::ECC_Camera;

	/** クエリプローブ球の半径（Unreal Unit） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Move|Collision", meta=(editcondition = "bCollisionTest"))
	float ProbeSize = 12.0f;
};

USTRUCT(BlueprintType)
struct COMMON3DCAMERA_API FC3DFadeSettings
{
	GENERATED_BODY()

	/**
	* オブジェクトを完全に表示するまでの時間（マテリアルパラメータ = MaterialFadeMaxValueの場合）
	*
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fade", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float FadeInTime = 0.8f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fade", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float FadeOutTime = 0.8f;

	/** フェード処理用のマテリアルパラメータ。カメラモード間の変更には非対応 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fade")
	float MaterialFadeMinValue = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fade")
	float MaterialFadeMaxValue = 1.f;

	/** 
	* 最小値@MaterialFadeMinValueまたは最大値@MaterialFadeMaxValueを設定するマテリアルパラメータ群。
	* これらのパラメータ（通常は1つ）でオブジェクトの表示・非表示を制御する。
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fade")
	TArray<FName> MaterialFadeParamNames;

	/** 
	* カメラとキャラクター間のオブジェクトを検索するトレースで使用するチャンネル。
	* @UC3DCameraFadingObject::FindCollidedActorsを参照。
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fade")
	TEnumAsByte<ECollisionChannel> FadeChannel = ECC_Camera;

	/** 
	* 自身のキャラクターがカメラと衝突した場合に非表示化するか。
	* カメラの判定半径は@SelfFadeCheckRadiusを参照。
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fade")
	bool bFadeSelfIfCollision = true;

	/** trueの場合は@SelfFadeCustomFadeOutTime、それ以外は@FadeOutTimeを使用する */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fade", meta = (InlineEditConditionToggle))
	bool bUseCustomFadeOutTimeForSelfFade = false;

	/** @bUseCustomFadeOutTimeForSelfFadeがtrueの場合のフェード時間。@FadeOutTimeを上書きする */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fade", meta = (EditCondition = "bUseCustomFadeOutTimeForSelfFade", ClampMin = "0.0", UIMin = "0.0"))
	float SelfFadeCustomFadeOutTime = 1.f;

	/** カメラとキャラクターの衝突判定に使用する球形トレースの半径 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fade", meta = (EditCondition = "bFadeSelfIfCollision", ClampMin = "1.0", UIMin = "1.0"))
	float SelfFadeCheckRadius = 18.f;

	/** trueかつbFadeSelfIfCollisionがtrueの場合、キャラクターにアタッチされた全アクターをフェードする */
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

	/** エラーをログと画面に出力する。falseの場合はログのみに出力する */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Logs")
	bool bPrintErrorsToScreen = true;

	/**
	* デバッグ対象：CameraLocationObject
	* 位置オブジェクトのデバッグ情報を表示する
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	bool bEnableMoveObjectDebug = false;

	/**
	* デバッグ対象：CameraFadingObject
	* カメラとキャラクター間のオブジェクトを検索するデバッグトレースを表示する
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	bool bEnableDrawFadeShapeDebug = false;
};
