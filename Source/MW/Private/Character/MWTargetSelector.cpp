#include "Character/MWTargetSelector.h"
#include "Gameplay/MWGameplayUtility.h"
#include "Gameplay/Battle/MWBattleSystem.h"

FMWTargetSelector::FMWTargetSelector(const AController* InController)
{
	Controller = InController;
	OwnerType = Controller.IsA(APlayerController::StaticClass()) ? MWTargetSelector::Player : MWTargetSelector::Npc;
}

FMWTargetSelector::~FMWTargetSelector()
{

}

void FMWTargetSelector::SwitchToLeft()
{
	TryFindSelectableTarget(true);
}

void FMWTargetSelector::SwitchToRight()
{
	TryFindSelectableTarget(false);
}

void FMWTargetSelector::CancelSelect()
{
	if (!SelectedTarget.IsValid())
	{
		return;
	}

	SelectedTarget.Reset();
	if (UMWBattleSystem* mwbs = UMWBattleSystem::Get(Controller))
	{
		if (mwbs->OnTargetCancelSelected.IsBound())
		{
			mwbs->OnTargetCancelSelected.Broadcast(SelectedTarget);
		}
	}
}

void FMWTargetSelector::LockTarget()
{
	//if (LockedTarget.IsValid())
	//{
	//	return;
	//}

	if (!Controller)
	{
		return;
	}

	if (!SelectedTarget.IsValid())
	{
		if (OwnerType == MWTargetSelector::Player)
		{
			TArray<AActor*> ignoreActors;
			ignoreActors.Emplace(Controller->GetPawn());
			UWMGameplayUtility::SearchSelectableTargets(Cast<const APlayerController>(Controller), FindTargets, ignoreActors);
		}
		else
		{
			// TODO : when owner is npc, how to search target
		}

		// if no targets found, then do nothing
		if (FindTargets.Num() == 0)
		{
			return;
		}

		// try to find a nearest target
		SelectedTarget = GetNearestTarget(FindTargets);

		if (!SelectedTarget.IsValid())
		{
			return;
		}
	}

	LockedTarget = SelectedTarget;

	// broadcast lock target event
	if (UMWBattleSystem* mwbs = UMWBattleSystem::Get(Controller))
	{
		if (mwbs->OnTargetLocked.IsBound())
		{
			mwbs->OnTargetLocked.Broadcast(LockedTarget);
		}
	}
}

void FMWTargetSelector::UnlockTarget()
{
	if (!Controller)
	{
		return;
	}

	if (LockedTarget.IsValid())
	{
		LockedTarget.Reset();
		
		// broadcast unlock target event
		if (UMWBattleSystem* mwbs = UMWBattleSystem::Get(Controller))
		{
			if (mwbs->OnTargetUnlocked.IsBound())
			{
				mwbs->OnTargetUnlocked.Broadcast(LockedTarget);
			}
		}
	}
}

void FMWTargetSelector::TryFindSelectableTarget(bool bLeft)
{
	if (!Controller)
	{
		return;
	}

	if (OwnerType == MWTargetSelector::Player)
	{
		TArray<AActor*> ignoreActors;
		ignoreActors.Emplace(Controller->GetPawn());
		UWMGameplayUtility::SearchSelectableTargets(Cast<const APlayerController>(Controller), FindTargets, ignoreActors);
	}
	else
	{
		// TODO : when owner is npc, how to search target
	}

	// if no targets found, then do nothing
	if (FindTargets.Num() == 0)
	{
		return;
	}
	
	// check if there is a current target
	if (!SelectedTarget.IsValid())
	{
		// if current target is not existed, then get the nearest target
		SelectedTarget = GetNearestTarget(FindTargets);
	}
	else
	{
		// there is a current target,
		// if it's in the found targets, get the object next to the current target
		// if it's not in the found targets, get the nearest object in the container
		if (FindTargets.Contains(SelectedTarget))
		{
			SelectedTarget = GetTargetNextToSelectedTarget(FindTargets, bLeft);
		}
		else
		{
			SelectedTarget = GetNearestTarget(FindTargets);
		}	
	}

	// double check if target is found
	if (!SelectedTarget.IsValid())
	{
		return;
	}

	if (UMWBattleSystem* mwbs = UMWBattleSystem::Get(Controller))
	{
		if (mwbs->OnTargetSelected.IsBound())
		{
			mwbs->OnTargetSelected.Broadcast(SelectedTarget);
		}
	}
}

FMWFoundActorInfo FMWTargetSelector::GetNearestTarget(TArray<FMWFoundActorInfo>& Targets)
{
	Targets.Sort([](const FMWFoundActorInfo& lhs, const FMWFoundActorInfo& rhs) {
		return lhs.bLeft == rhs.bLeft ? lhs.Angle < rhs.Angle : lhs.bLeft;
		});
	return Targets[0];
}

FMWFoundActorInfo FMWTargetSelector::GetTargetNextToSelectedTarget(TArray<FMWFoundActorInfo>& Targets, bool bLeft)
{
	Targets.Sort([](const FMWFoundActorInfo& lhs, const FMWFoundActorInfo& rhs) {
		return lhs.bLeft == rhs.bLeft ? lhs.Angle < rhs.Angle : lhs.bLeft;
		});
	int32 id = Targets.IndexOfByKey(SelectedTarget);

	if (bLeft)
	{
		// if current target is the leftmost, then switch to rightmost 
		if (id == 0)
		{
			id = Targets.Num() - 1;
		}
		else
		{
			id -= 1;
		}
	}
	else
	{
		// if current target is the rightmost, then switch to leftmost
		if (id == Targets.Num() - 1)
		{
			id = 0;
		}
		else
		{
			id += 1;
		}
	}
	return Targets[id];
}

void FMWTargetSelector::ForceLockIfNoTarget(const FMWFoundActorInfo& Target)
{
	if (LockedTarget.IsValid())
	{
		return;
	}

	LockedTarget = Target;
	// broadcast lock target event
	if (UMWBattleSystem* mwbs = UMWBattleSystem::Get(Controller))
	{
		if (mwbs->OnTargetLocked.IsBound())
		{
			mwbs->OnTargetLocked.Broadcast(LockedTarget);
		}
	}
}

void FMWTargetSelector::OnTargetNotExisted(const FMWFoundActorInfo& Target)
{
	if (Target == LockedTarget)
	{
		UnlockTarget();
	}

	if (Target == SelectedTarget)
	{
		CancelSelect();
	}
}

void FMWTargetSelector::ChangeOwnerController(const AController* NewController)
{
	Controller = NewController;
	OwnerType = Controller.IsA(APlayerController::StaticClass()) ? MWTargetSelector::Player : MWTargetSelector::Npc;
}

bool FMWTargetSelector::HasSelectedTarget() const
{
	return SelectedTarget.IsValid();
}

bool FMWTargetSelector::HasLockedTarget() const
{
	return LockedTarget.IsValid();
}
