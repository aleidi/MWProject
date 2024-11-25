#include "Battle/MWWeakSystem.h"
#include "Subsystem/MWBattleSystem.h"

DEFINE_LOG_CATEGORY_STATIC(LogMWWeakSystem, All, All)

UMWWeakSystem::UMWWeakSystem()
{
	/*if (UMWBattleSystem* mwbs = GetWorld()->GetSubsystem<UMWBattleSystem>())
	{
		mwbs->OnWeakTagAssigned.AddDynamic(this, &UMWWeakSystem::OnNewWeakTagAdded);
	}*/
}

void UMWWeakSystem::SetOwner(UObject* NewOwner)
{
	Owner = NewOwner;

	if (UMWBattleSystem* mwbs = GetWorld()->GetSubsystem<UMWBattleSystem>())
	{
		mwbs->TryTriggerWeak.AddDynamic(this, &UMWWeakSystem::TryTriggerWeak);
	}
}

bool UMWWeakSystem::CheckAndTriggerWeak(const FGameplayTag& NewTag, int32 Value)
{
	if (!bCanTrigger)
	{
		return false;
	}

	switch (WeakState)
	{
	case EWeakState::Normal:
		// Check which combo matches the tag and trigger it
		for (int8 i = 0; i < WeakPointCombos.Num(); ++i)
		{
			if (WeakPointCombos[i]->TryTrigger(NewTag, Value,
				[&](FWeakTriggerInfo& TriggerInfo) 
				{
					WeakState = EWeakState::Weak;
					bCanTrigger = !TriggerInfo.bLast;
					TriggerTimes = TriggerInfo.Times;
					WeakenProcess(TriggerInfo.Duration);
				}))
			{
				CurrentComboId = i;

				return true;
			}
		}
		break;
	case EWeakState::Weak:
		if (WeakPointCombos[CurrentComboId]->TryTrigger(NewTag, Value,
			[&](FWeakTriggerInfo& TriggerInfo)
			{
				bCanTrigger = !TriggerInfo.bLast;
				TriggerTimes = TriggerInfo.Times;
				WeakenProcess(TriggerInfo.Duration);
			}))
		{

			return true;
		}
		break;
	case EWeakState::Restore:
		break;
	}

	return false;
}

const TArray<UWeakPointCombo*>& UMWWeakSystem::GetWeakPointCombos() const
{
	return WeakPointCombos;
}

void UMWWeakSystem::TryTriggerWeak(UObject* InOwner, const FGameplayTag& NewTag, int32 Value)
{
	if (InOwner == Owner && NewTag.IsValid())
	{
		CheckAndTriggerWeak(NewTag, Value);
	}
}

void UMWWeakSystem::OnWeakRemoved(UObject* InOwner, const FGameplayTag& RemovedTag)
{
	if (InOwner == Owner)
	{
	}
}

void UMWWeakSystem::WeakenProcess(int32 Duration)
{
	// notify weak is triggered
	if (auto* mwbs = GetWorld()->GetSubsystem<UMWBattleSystem>())
	{
		if (mwbs->OnWeakTriggered.IsBound())
		{
			mwbs->OnWeakTriggered.Broadcast(Owner, TriggerTimes);
		}
	}

	// check the weak timer, reset if it's used
	if (thWeakRestore.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(thWeakRestore);
	}

	GetWorld()->GetTimerManager().SetTimer(thWeakRestore, this, &UMWWeakSystem::WeakRestore, (float)Duration);
}

void UMWWeakSystem::WeakRestore()
{
	WeakState = EWeakState::Restore;
	CurrentComboId = -1;

	if (thWeakRestore.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(thWeakRestore);
	}

	EnterImmune();
}

void UMWWeakSystem::EnterImmune()
{
	if (thImmune.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(thImmune);
	}
	GetWorld()->GetTimerManager().SetTimer(thImmune, this, &UMWWeakSystem::ExitImmune, (float)CoolDown);
}

void UMWWeakSystem::ExitImmune()
{
	WeakState = EWeakState::Normal;
}

bool UWeakPointCombo::TryTrigger(FGameplayTag WeakTag, int32 Value, TFunction<void(FWeakTriggerInfo&)> TriggeredCallback)
{
	if (!IsValid())
	{
		return false;
	}

	if (WeakTag == Combo[Current].TriggerTag)
	{
		if (!Combo[Current].TryTrigger(Value))
		{
			return false;
		}

		FWeakTriggerInfo info;
		info.bLast = (Current == Combo.Num() - 1);
		Current = info.bLast ? 0 : Current + 1;
		info.Duration = Combo[Current].Duration;
		info.Times = Current + 1;
		TriggeredCallback(info);

		return true;
	}
	
	return false;
}

bool FWeakPoint::TryTrigger(int32 Value)
{
	Exposure += Value;

	if (Exposure >= MW_WEAK_EXPOSURE_MAX)
	{
		Exposure = MW_WEAK_EXPOSURE_MIN;
		return true;
	}

	return false;
}

FWeakPoint::FWeakPoint()
	:Exposure(MW_WEAK_EXPOSURE_MIN)
{
}
