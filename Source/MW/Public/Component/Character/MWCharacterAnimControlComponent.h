#pragma once

// Include Header
#include "Component/Pawn/MWPawnComponent.h"
#include "MWCharacterAnimControlComponent.generated.h"

// Forward Declare

// Define

/*
 * @class UMWCharacterAnimControlComponent
 * 
 * @brief
 *
 * @note
 */
UCLASS(BlueprintType, meta=(BlueprintSpawnableComponent))
class UMWCharacterAnimControlComponent : public UMWPawnComponent
{
	GENERATED_BODY()
	
public:
	UMWCharacterAnimControlComponent(const FObjectInitializer& ObjectInitializer);

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

//====================================//
//        Approach Target             //
//====================================//
protected:
	void DoApproach(float DeltaTime);

	bool CanApproach() const;

	void ResetApproachProgress();

	/**
	 * Checks and sets the owner's animation instance.
	 * オンナーのアニメーションインスタンスを確認して設定する。
	 *
	 * @return true if the animation instance is valid and 'OwnerAnimInst' is set, false otherwise.
	 *         trueの場合、アニメーションインスタンスが有効で 'OwnerAnimInst'も有効に設定されている。そうでない場合はfalse。
	 */
	bool CheckAndSetOwnerAnimInst();

	void DrawDebugShapeForApprachTarget() const;

public:
	// Sets the target actor to approach and starts approaching.
	// ターゲットアクターを設定し、接近を開始する。
	UFUNCTION(BlueprintCallable, Category = "ApproachTarget")
	void StartApproachTarget(const AActor* InTarget);

	// Sets the point to approach and starts approaching.
	// This method don't consider the collision of target and owner.
	// 接近するポイントを設定し、接近を開始する。
	// 'StartApproachTarget'に比べて、この方法はターゲットとオーナーのコリジョンを考慮しない。
	UFUNCTION(BlueprintCallable, Category = "ApproachTarget")
	void StartApproachPoint(const FVector& InPoint);

	// Force stop approaching.
	// 強制的に接近を停止する。
	UFUNCTION(BlueprintCallable, Category = "ApproachTarget")
	void EndApproach(bool bForceTeleportToDestination = true);

	bool IsDoingApproach() const;

protected:
	// Name of the animation curve used for approaching the target.
	// ターゲットに接近するために使用されるアニメーションカーブの名前。
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ApproachTarget", meta = (DisplayName="CurveName"))
	FName ApproachTargetCurveName = FName("ApproachTarget");

	// Tolerance distance to consider the approach to the target completed.
	// ターゲットへの接近が完了と見なす許容距離。
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ApproachTarget")
	float ApproachEndLocTolerance = 10.f;

	// Offset distance from the target's collision point to stop approaching, used when collision is not available.
	// コリジョンが利用できない場合に使用される、ターゲットのコリジョンポイントから接近を停止するためのオフセット距離。
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ApproachTarget")
	float ApproachDistanceOffset = 150.f;

	// Collision channel used for searching the closest point on the target during approach.
	// 接近中にターゲット上の最も近いポイントを検索するために使用されるコリジョンチャネル。
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ApproachTarget")
	TEnumAsByte<ECollisionChannel> ApproachTraceChannel = ECC_Camera;

	UPROPERTY(BlueprintReadOnly, Category = "ApproachTarget")
	TObjectPtr<const AActor> ApproachTarget;

	UPROPERTY(BlueprintReadOnly, Category = "ApproachTarget")
	float ApproachProgress = 0.f;

	UPROPERTY()
	TObjectPtr<UAnimInstance> OwnerAnimInst;

	UPROPERTY()
	FVector ApproachStartLocation;

	UPROPERTY()
	FVector ApproachEndLocation;

	UPROPERTY(BlueprintReadOnly, Category = "ApproachTarget")
	bool bCanDoApproach = false;

};