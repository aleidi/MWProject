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
	// bFadeOutがtrueの場合は非表示化
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

	// トレースで衝突アクターを検索
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
	/** フェード設定プロパティ */
	bool bEnableFade = true;

	float FadeInTime = 0.8f;
	float FadeOutTime = 0.8f;

	/** フェード処理用のマテリアルパラメータ。カメラモード間の変更には非対応 */

	float MaterialFadeMinValue = 0.f;
	float MaterialFadeMaxValue = 1.f;

	/** 
	* 最小値@MaterialFadeMinValueまたは最大値@MaterialFadeMaxValueを設定するマテリアルパラメータ群。
	* これらのパラメータ（通常は1つ）でオブジェクトの表示・非表示を制御する。
	*/
	TArray<FName> MaterialFadeParamNames;

	/** 
	* カメラとキャラクター間のオブジェクトを検索するトレースで使用するチャンネル。
	* @UC3DCameraFadingObject::FindCollidedActorsを参照。
	*/
	ECollisionChannel FadeChannel = ECC_Camera;

	/** 
	* 自身のキャラクターがカメラと衝突した場合に非表示化するか。
	* カメラの判定半径は@SelfFadeCheckRadiusを参照。
	*/
	bool bFadeSelfIfCollision = true;

	/** trueの場合は@SelfFadeCustomFadeOutTime、それ以外は@FadeOutTimeを使用する */
	bool bUseCustomFadeOutTimeForSelfFade = false;

	/** @bUseCustomFadeOutTimeForSelfFadeがtrueの場合のフェード時間。@FadeOutTimeを上書きする */
	float SelfFadeCustomFadeOutTime = 1.f;

	/** カメラとキャラクターの衝突判定に使用する球形トレースの半径 */
	float SelfFadeCheckRadius = 18.f;

	/** trueかつbFadeSelfIfCollisionがtrueの場合、キャラクターにアタッチされた全アクターをフェードする */
	bool bSelfFadeAttachedActors = true;
};
