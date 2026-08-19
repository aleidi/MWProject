#pragma once

#include "MW.h"
#include "Define/MWDefineGameplay.h"

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
	FMWTargetSelector() = default;
	FMWTargetSelector(const AController* InController);
	virtual ~FMWTargetSelector();

	void SwitchToLeft();
	void SwitchToRight();
	void CancelSelect();
	void LockTarget();
	void UnlockTarget();
	void ForceLockIfNoTarget(const FMWFoundActorInfo& Target);
	void OnTargetNotExisted(const FMWFoundActorInfo& Target);
	void ChangeOwnerController(const AController* NewController);

	bool HasSelectedTarget() const;
	bool HasLockedTarget() const;

private:
	/* Leftがtrueの場合は現在のターゲットの左隣、falseの場合は右隣を新たなターゲットにします。 */
	void TryFindSelectableTarget(bool bLeft);
	FMWFoundActorInfo GetNearestTarget(TArray<FMWFoundActorInfo>& Targets);
	FMWFoundActorInfo GetTargetNextToSelectedTarget(TArray<FMWFoundActorInfo>& Targets, bool bLeft);

private:
	TObjectPtr<const AController> Controller;
	TArray<FMWFoundActorInfo> FindTargets;
	FMWFoundActorInfo SelectedTarget;
	FMWFoundActorInfo LockedTarget;
	/* 所有者に応じて検索方式を切り替えます。 */
	MWTargetSelector::OwnerType OwnerType;
};