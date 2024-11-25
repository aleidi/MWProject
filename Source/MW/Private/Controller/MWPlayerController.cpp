#include "Controller/MWPlayerController.h"
#include "Subsystem/MWBattleSystem.h"
#include "Controller/MWTargetSelector.h"

AMWPlayerController::AMWPlayerController()
{

}

void AMWPlayerController::SwitchToLeft()
{
	if (TargetSearcher)
	{
		TargetSearcher->SwitchToLeft();
	}
}

void AMWPlayerController::SwitchToRight()
{
	if (TargetSearcher)
	{
		TargetSearcher->SwitchToRight();
	}
}

void AMWPlayerController::CancelSelect()
{
	if (TargetSearcher)
	{
		TargetSearcher->CancelSelect();
	}
}

void AMWPlayerController::LockTarget()
{
	if (TargetSearcher)
	{
		TargetSearcher->LockTarget();
	}
}

void AMWPlayerController::UnlockTarget()
{
	if (TargetSearcher)
	{
		TargetSearcher->UnlockTarget();
	}
}

void AMWPlayerController::NotifyPlayerAttackted(FMWActorInfo Attacker, FMWActorInfo Attackee)
{
	if (TargetSearcher && Attackee.Name == GetPawn()->GetName())
	{

		TargetSearcher->ForceLockIfNoTarget(Attacker);
	}
}

void AMWPlayerController::OnOtherCharacterDied(FMWActorInfo InCharacter)
{
	if (TargetSearcher && InCharacter.IsValid() && InCharacter.Name != GetCharacter()->GetName())
	{

		TargetSearcher->OnTargetNotExisted(InCharacter);
	}
}

void AMWPlayerController::BeginPlay()
{
	Super::BeginPlay();

	TargetSearcher = MakeShared<FMWTargetSelector>(this);

	
}

void AMWPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	TargetSearcher.Reset();
}

void AMWPlayerController::BindDelegates()
{
	if (UMWBattleSystem* mwbs = GetWorld()->GetSubsystem<UMWBattleSystem>())
	{
		mwbs->OnAttack.AddDynamic(this, &AMWPlayerController::NotifyPlayerAttackted);
		mwbs->OnCharacterDied.AddDynamic(this, &AMWPlayerController::OnOtherCharacterDied);
	}
}
