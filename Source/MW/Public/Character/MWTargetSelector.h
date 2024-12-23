#pragma once

#include "CoreMinimal.h"
#include "Gameplay/MWGameplayTypes.h"

namespace MWTargetSelector
{
	enum OwnerType
	{
		Player,
		Npc
	};
}

class FMWTargetSelector : public TSharedFromThis<FMWTargetSelector>
{
public:
	FMWTargetSelector() = delete;
	FMWTargetSelector(AController* InController);

	void SwitchToLeft();
	void SwitchToRight();
	void CancelSelect();
	void LockTarget();
	void UnlockTarget();
	void ForceLockIfNoTarget(const FMWFoundActorInfo& Target);
	void OnTargetNotExisted(const FMWFoundActorInfo& Target);
	void ChangeOwnerController(AController* NewController);

	bool HasSelectedTarget() const;
	bool HasLockedTarget() const;

private:
	/* Left - if true, the new target is left-next to current target, otherwise the new target is right-next to current target. */
	void TryFindSelectableTarget(bool bLeft);
	FMWFoundActorInfo GetNearestTarget(TArray<FMWFoundActorInfo>& Targets);
	FMWFoundActorInfo GetTargetNextToSelectedTarget(TArray<FMWFoundActorInfo>& Targets, bool bLeft);

private:
	TObjectPtr<AController> Controller;
	TArray<FMWFoundActorInfo> FindTargets;
	FMWFoundActorInfo SelectedTarget;
	FMWFoundActorInfo LockedTarget;
	/* Using different search method for different owner. */
	MWTargetSelector::OwnerType OwnerType;
};