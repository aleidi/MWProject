#pragma once

#include "CoreMinimal.h"
#include "Gameplay/MWGameplayTypes.h"

class FMWTargetSelector : public TSharedFromThis<FMWTargetSelector>
{
public:
	FMWTargetSelector() = delete;
	FMWTargetSelector(class APlayerController* Controller);

	void SwitchToLeft();
	void SwitchToRight();
	void CancelSelect();
	void LockTarget();
	void UnlockTarget();
	void ForceLockIfNoTarget(const FMWActorInfo& Target);
	void OnTargetNotExisted(const FMWActorInfo& Target);

private:
	/* Left - if true, the new target is left-next to current target, otherwise the new target is right-next to current target. */
	void TryFindSelectableTarget(bool bLeft);
	FMWActorInfo GetNearestTarget(TArray<FMWActorInfo>& Targets);
	FMWActorInfo GetTargetNextToSelectedTarget(TArray<FMWActorInfo>& Targets, bool bLeft);
private:
	bool bSelectTarget;
	TObjectPtr<class APlayerController> PlayerController;
	TArray<FMWActorInfo> FindTargets;
	FMWActorInfo SelectedTarget;
	FMWActorInfo LockedTarget;
};