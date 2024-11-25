#include "Controller/MWTargetSelector.h"
#include "Gameplay/MWGameplayUtility.h"
#include "Subsystem/MWBattleSystem.h"

FMWTargetSelector::FMWTargetSelector(APlayerController* Controller)
	:bSelectTarget(false)
{
	ensure(Controller != nullptr);
	PlayerController = Controller;
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
	if (UMWBattleSystem* mwbs = PlayerController->GetWorld()->GetSubsystem<UMWBattleSystem>())
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

	if (!PlayerController)
	{
		return;
	}

	if (!SelectedTarget.IsValid())
	{
		UWMGameplayUtility::SearchSelectableTargets(PlayerController, FindTargets);

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
	if (UMWBattleSystem* mwbs = PlayerController->GetWorld()->GetSubsystem<UMWBattleSystem>())
	{
		if (mwbs->OnTargetLocked.IsBound())
		{
			mwbs->OnTargetLocked.Broadcast(LockedTarget);
		}
	}
}

void FMWTargetSelector::UnlockTarget()
{
	if (!PlayerController)
	{
		return;
	}

	if (LockedTarget.IsValid())
	{
		LockedTarget.Reset();
		
		// broadcast unlock target event
		if (UMWBattleSystem* mwbs = PlayerController->GetWorld()->GetSubsystem<UMWBattleSystem>())
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
	if (!PlayerController)
	{
		return;
	}

	UWMGameplayUtility::SearchSelectableTargets(PlayerController, FindTargets);

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

	if (UMWBattleSystem* mwbs = PlayerController->GetWorld()->GetSubsystem<UMWBattleSystem>())
	{
		if (mwbs->OnTargetSelected.IsBound())
		{
			mwbs->OnTargetSelected.Broadcast(SelectedTarget);
		}
	}
}

FMWActorInfo FMWTargetSelector::GetNearestTarget(TArray<FMWActorInfo>& Targets)
{
	Targets.Sort([](const FMWActorInfo& lhs, const FMWActorInfo& rhs) {
		return lhs.bLeft == rhs.bLeft ? lhs.Angle < rhs.Angle : lhs.bLeft;
		});
	return Targets[0];
}

FMWActorInfo FMWTargetSelector::GetTargetNextToSelectedTarget(TArray<FMWActorInfo>& Targets, bool bLeft)
{
	Targets.Sort([](const FMWActorInfo& lhs, const FMWActorInfo& rhs) {
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

void FMWTargetSelector::ForceLockIfNoTarget(const FMWActorInfo& Target)
{
	if (LockedTarget.IsValid())
	{
		return;
	}

	LockedTarget = Target;
	// broadcast lock target event
	if (UMWBattleSystem* mwbs = PlayerController->GetWorld()->GetSubsystem<UMWBattleSystem>())
	{
		if (mwbs->OnTargetLocked.IsBound())
		{
			mwbs->OnTargetLocked.Broadcast(LockedTarget);
		}
	}
}

void FMWTargetSelector::OnTargetNotExisted(const FMWActorInfo& Target)
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
