#include "Component/Character/MWCharacterAnimControlComponent.h"
#include "Components/CapsuleComponent.h"

UMWCharacterAnimControlComponent::UMWCharacterAnimControlComponent(const FObjectInitializer& ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = TG_PostPhysics;
}

void UMWCharacterAnimControlComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (IsApproaching())
	{
		DoApproach(DeltaTime);
	}
}

void UMWCharacterAnimControlComponent::DoApproach(float DeltaTime)
{
	// No animation curve found, stop approaching.
	// アニメーションカーブが見つからない、接近を中止.
	bool bHasCurve = OwnerAnimInst->GetCurveValue(ApproachTargetCurveName, ApproachProgress);
	if(FMath::IsNearlyZero(ApproachProgress))
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Approach progress : %f"), ApproachProgress));
	}
	if (!bHasCurve)
	{
		return;
	}

	FVector targetLoc = FMath::Lerp(ApproachStartLocation, ApproachEndLocation, ApproachProgress);
	// Ignore Z axis change.
	// Z軸の変化を無視する.
	targetLoc.Z = ApproachStartLocation.Z;

	GetOwner()->SetActorLocation(targetLoc);

	if (ApproachProgress >= 0.99f || FVector::Dist(targetLoc, ApproachEndLocation) < ApproachEndLocTolerance)
	{
		EndApproach(true);
		return;
	}
}

void UMWCharacterAnimControlComponent::ResetApproachProgress()
{
	ApproachProgress = 0.f;
}

bool UMWCharacterAnimControlComponent::CheckAndSetOwnerAnimInst()
{
	// No animation instance in owner, stop approaching.
	// オーナーにアニメーションインスタンス無し、接近を禁止.
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

float UMWCharacterAnimControlComponent::GetDistanceBetweenCapsules(const AActor* SourceActor, const AActor* TargetActor, FVector& OutClosestPoint)
{
	if (!SourceActor || !TargetActor)
	{
		return -1.f;
	}

	auto* sourceCapsule = SourceActor->FindComponentByClass<UCapsuleComponent>();
	auto* targetCapsule = TargetActor->FindComponentByClass<UCapsuleComponent>();
	if (!sourceCapsule || !targetCapsule)
	{
		return -1.f;
	}

	const float sourceRadius = sourceCapsule->GetScaledCapsuleRadius();
	const float targetRadius = targetCapsule->GetScaledCapsuleRadius();

	FVector sourceLocation = SourceActor->GetActorLocation();
	FVector targetLocation = TargetActor->GetActorLocation();

	const float disOfActor = FVector::Dist(sourceLocation, targetLocation);
	if(disOfActor <= sourceRadius + targetRadius)
	{
		return -1.f;
	}

	FVector dir = (sourceLocation - targetLocation).GetSafeNormal();

	OutClosestPoint = targetLocation + dir * (targetRadius + sourceRadius);

	return FVector::Dist(OutClosestPoint, sourceLocation);
}

void UMWCharacterAnimControlComponent::StartApproachTarget(const AActor* InTarget)
{
	// If the owner is doing approach, stop approaching.
	if (IsApproaching())
	{
		return;
	}

	// No target, stop approaching.
	// ターゲット無し、接近を禁止.
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
	// ターゲットとポーンのコリジョンから距離アポローチ始点と終点を計算する.
	// コリジョンが利用できない場合は 'ApproachDistanceOffset' を使用する.

	// Calculate ApproachEndLocation
	// ApproachEndLocationを計算する
	float distance = GetDistanceBetweenCapsules(pawn, ApproachTarget, ApproachEndLocation);

	// The distance between start and end is too small, stop approaching.
	// 開始と終了の距離が小さすぎる、接近を禁止.
	if(distance <= KINDA_SMALL_NUMBER)
	{
		ApproachTarget = nullptr;
		OwnerAnimInst = nullptr;

		return;
	}

	ResetApproachProgress();

	bIsApproaching = true;

	DrawDebugShapeForApprachTarget();
}

void UMWCharacterAnimControlComponent::StartApproachPoint(const FVector& InPoint)
{
	// If the owner is doing approach, stop approaching.
	if (IsApproaching())
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
	// 開始と終了の距離が小さすぎる、接近を禁止.
	if (distance <= KINDA_SMALL_NUMBER)
	{
		return;
	}

	ResetApproachProgress();

	bIsApproaching = true;

	DrawDebugShapeForApprachTarget();
}

void UMWCharacterAnimControlComponent::EndApproach(bool bForceTeleportToDestination)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, FString::Printf(TEXT("ApproachProgress end: %f"), ApproachProgress));

	// Ignore Z axis change.
	// Z軸の変化を無視する.
	if (bForceTeleportToDestination)
	{
		ApproachEndLocation.Z = ApproachStartLocation.Z;
		GetOwner()->SetActorLocation(ApproachEndLocation);
	}

	ApproachTarget = nullptr;

	OwnerAnimInst = nullptr;

	bIsApproaching = false;
}

bool UMWCharacterAnimControlComponent::IsApproaching() const
{
	return bIsApproaching;
}
