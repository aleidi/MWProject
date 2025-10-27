#include "Component/Character/MWCharacterAnimControlComponent.h"

UMWCharacterAnimControlComponent::UMWCharacterAnimControlComponent(const FObjectInitializer& ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = TG_PostPhysics;
}

void UMWCharacterAnimControlComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (CanApproach())
	{
		DoApproach(DeltaTime);
	}
}

void UMWCharacterAnimControlComponent::DoApproach(float DeltaTime)
{
	// No animation curve found, stop approaching.
	// アニメーションカーブが見つからない、接近を中止。
	bool bHasCurve = OwnerAnimInst->GetCurveValue(ApproachTargetCurveName, ApproachProgress);
	if (!bHasCurve)
	{
		return;
	}

	const FVector targetLoc = FMath::Lerp(ApproachStartLocation, ApproachEndLocation, ApproachProgress);

	GetOwner()->SetActorLocation(targetLoc);

	// Approaching target completed, clear the data.
	// ターゲットへの接近が完了、データをクリア。
	auto EndApproaching = [this]() -> void
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, FString::Printf(TEXT("ApproachProgress end: %f"), ApproachProgress));
		ApproachTarget = nullptr;
		OwnerAnimInst = nullptr;

		ResetApproachProgress();

		bCanDoApproach = false;

	};

	if(ApproachProgress > 0.99f)
	{
		EndApproaching();

		return;
	}

	const float distance = FVector::Dist(targetLoc, ApproachEndLocation);

	if (distance < ApproachEndLocTolerance)
	{
		EndApproaching();

		return;
	}
}

bool UMWCharacterAnimControlComponent::CanApproach() const
{
	return	bCanDoApproach;
}

void UMWCharacterAnimControlComponent::ResetApproachProgress()
{
	ApproachProgress = 0.f;
}

bool UMWCharacterAnimControlComponent::CheckAndSetOwnerAnimInst()
{
	// No animation instance in owner, stop approaching.
	// オーナーにアニメーションインスタンス無し、接近を禁止。
	auto* pawn = GetOwner();
	auto* skmc = pawn ? pawn->FindComponentByClass<USkeletalMeshComponent>() : nullptr;
	OwnerAnimInst = skmc ? skmc->GetAnimInstance() : nullptr;
	if (!OwnerAnimInst)
	{
		return false;
	}

	return true;
}

void UMWCharacterAnimControlComponent::DrawDebugShapeForApprachTarget() const
{
	::DrawDebugSphere(GetWorld(), ApproachEndLocation, 15.f, 12, FColor::Red, false, 5.f);
	::DrawDebugSphere(GetWorld(), ApproachStartLocation, 15.f, 12, FColor::Blue, false, 5.f);
	::DrawDebugDirectionalArrow(GetWorld(), ApproachStartLocation, ApproachEndLocation, 50.f, FColor::Green, false, 5.f, 0, 5.f);
}

void UMWCharacterAnimControlComponent::StartApproachTarget(AActor* InTarget)
{
	// If the owner is doing approach, stop approaching.
	if (bCanDoApproach)
	{
		return;
	}

	// No target, stop approaching.
	// ターゲット無し、接近を禁止。
	ApproachTarget = InTarget;
	if (!ApproachTarget)
	{
		return;
	}

	if (!CheckAndSetOwnerAnimInst())
	{
		ApproachTarget = nullptr;

		return;
	}

	auto* pawn = GetOwner();

	// Calculate approach start/end location
	// 接近開始/終了位置を計算
	ApproachStartLocation = pawn->GetActorLocation();

	ApproachEndLocation = ApproachTarget->GetActorLocation();

	// Calculate the approach start and end location from the collision of target and pawn.
	// Use 'ApproachDistanceOffset' and direction from pawn to target if collision is not available.
	// ターゲットとポーンのコリジョンから距離アポローチ始点と終点を計算。
	// コリジョンが利用できない場合は 'ApproachDistanceOffset' を使用。
	FVector cloestPointOnTarget;

	// Calculate ApproachEndLocation
	// ApproachEndLocationを計算
	float distance = ApproachTarget->ActorGetDistanceToCollision(ApproachStartLocation, ApproachTraceChannel, cloestPointOnTarget, nullptr);
	if (distance > 0.f)
	{
		ApproachEndLocation = cloestPointOnTarget;
	}
	else
	{
		const FVector approachDir = (ApproachEndLocation - ApproachStartLocation).GetSafeNormal();

		ApproachEndLocation = ApproachEndLocation - approachDir * ApproachDistanceOffset;
	}

	// Approach end location should consider the collision of owner.
	// 自分のコリジョンも考慮する必要がある。
	distance = pawn->ActorGetDistanceToCollision(ApproachEndLocation, ApproachTraceChannel, cloestPointOnTarget, nullptr);
	if (distance > 0.f)
	{
		const float startLocOffset = FVector::Dist(ApproachStartLocation, cloestPointOnTarget);

		const FVector approachDir = (ApproachEndLocation - ApproachStartLocation).GetSafeNormal();

		ApproachEndLocation = ApproachEndLocation - approachDir * startLocOffset;
	}
	else
	{
		const FVector approachDir = (ApproachEndLocation - ApproachStartLocation).GetSafeNormal();

		ApproachEndLocation = ApproachEndLocation - approachDir * ApproachDistanceOffset;
	}

	distance = FVector::Dist(ApproachStartLocation, ApproachEndLocation);

	// The distance between start and end is too small, stop approaching.
	// 開始と終了の距離が小さすぎる、接近を禁止。
	if(distance <= KINDA_SMALL_NUMBER)
	{
		ApproachTarget = nullptr;
		OwnerAnimInst = nullptr;

		return;
	}

	ResetApproachProgress();

	bCanDoApproach = true;

	DrawDebugShapeForApprachTarget();
}

void UMWCharacterAnimControlComponent::StartApproachPoint(const FVector& InPoint)
{
	// If the owner is doing approach, stop approaching.
	if (bCanDoApproach)
	{
		return;
	}

	if (!CheckAndSetOwnerAnimInst())
	{
		return;
	}

	auto* pawn = GetOwner();

	ApproachStartLocation = pawn->GetActorLocation();

	ApproachEndLocation = InPoint;

	const float distance = FVector::Dist(ApproachStartLocation, ApproachEndLocation);

	// The distance between start and end is too small, stop approaching.
	// 開始と終了の距離が小さすぎる、接近を禁止。
	if (distance <= KINDA_SMALL_NUMBER)
	{
		return;
	}

	ResetApproachProgress();

	bCanDoApproach = true;

	DrawDebugShapeForApprachTarget();
}
