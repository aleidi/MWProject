#include "Character/MWTargetSelector.h"
#include "Gameplay/MWGameplayUtility.h"

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
			// TODO: 所有者がNPCの場合のターゲット検索方法を検討
		}

		// ターゲットが見つからなければ終了
		if (FindTargets.Num() == 0)
		{
			return;
		}

		// 最寄りのターゲットを検索
		SelectedTarget = GetNearestTarget(FindTargets);

		if (!SelectedTarget.IsValid())
		{
			return;
		}
	}

	LockedTarget = SelectedTarget;

	// ターゲットロックイベントを通知
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
		
		// ターゲットロック解除イベントを通知
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
		// TODO: 所有者がNPCの場合のターゲット検索方法を検討
	}

	// ターゲットが見つからなければ終了
	if (FindTargets.Num() == 0)
	{
		return;
	}
	
	// 現在のターゲットの有無を確認
	if (!SelectedTarget.IsValid())
	{
		// 現在のターゲットがなければ最寄りを選択
		SelectedTarget = GetNearestTarget(FindTargets);
	}
	else
	{
		// 現在のターゲットが検索結果に含まれる場合は隣接ターゲットを取得
		// 含まれない場合はコンテナ内の最寄りターゲットを取得
		if (FindTargets.Contains(SelectedTarget))
		{
			SelectedTarget = GetTargetNextToSelectedTarget(FindTargets, bLeft);
		}
		else
		{
			SelectedTarget = GetNearestTarget(FindTargets);
		}	
	}

	// ターゲットの取得結果を再確認
	if (!SelectedTarget.IsValid())
	{
		return;
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
		// 現在のターゲットが左端なら右端へ切り替え
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
		// 現在のターゲットが右端なら左端へ切り替え
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
	// ターゲットロックイベントを通知
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
