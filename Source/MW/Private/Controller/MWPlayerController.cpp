#include "Controller/MWPlayerController.h"
#include "Subsystem/MWBattleSystem.h"
#include "Character/MWTargetSelector.h"
#include "GameFramework/Character.h"
#include "Character/MWCharacter.h"

AMWPlayerController::AMWPlayerController()
{

}

AMWCharacter* AMWPlayerController::GetMWCharacter() const
{
	return Cast<AMWCharacter>(GetPawn());
}

void AMWPlayerController::SwitchToLeft()
{
	if (auto* character = GetMWCharacter())
	{
		auto selector = character->GetTargetSelector();
		if (selector.IsValid())
		{
			selector.Pin()->SwitchToLeft();
		}
	}
}

void AMWPlayerController::SwitchToRight()
{
	if (auto* character = GetMWCharacter())
	{
		auto selector = character->GetTargetSelector();
		if (selector.IsValid())
		{
			selector.Pin()->SwitchToRight();
		}
	}
}

void AMWPlayerController::CancelSelect()
{
	if (auto* character = GetMWCharacter())
	{
		auto selector = character->GetTargetSelector();
		if (selector.IsValid())
		{
			selector.Pin()->CancelSelect();
		}
	}
}

void AMWPlayerController::LockTarget()
{
	if (auto* character = GetMWCharacter())
	{
		auto selector = character->GetTargetSelector();
		if (selector.IsValid())
		{
			selector.Pin()->LockTarget();
		}
	}
}

void AMWPlayerController::UnlockTarget()
{
	if (auto* character = GetMWCharacter())
	{
		auto selector = character->GetTargetSelector();
		if (selector.IsValid())
		{
			selector.Pin()->UnlockTarget();
		}
	}
}


void AMWPlayerController::NotifyPlayerAttackted(FMWFoundActorInfo Attacker, FMWFoundActorInfo Attackee)
{
	if (Attackee.Name == GetPawn()->GetName())
	{
		if (auto* character = GetMWCharacter())
		{
			auto selector = character->GetTargetSelector();
			if (selector.IsValid())
			{
				selector.Pin()->ForceLockIfNoTarget(Attacker);
			}
		}
	}
}

void AMWPlayerController::OnOtherCharacterDied(FMWFoundActorInfo InCharacter)
{
	if (InCharacter.IsValid() && InCharacter.Name != GetCharacter()->GetName())
	{
		if (auto* character = GetMWCharacter())
		{
			auto selector = character->GetTargetSelector();
			if (selector.IsValid())
			{
				selector.Pin()->OnTargetNotExisted(InCharacter);
			}
		}
	}
}

void AMWPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AMWPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AMWPlayerController::BindDelegates()
{
	if (UMWBattleSystem* mwbs = GetWorld()->GetSubsystem<UMWBattleSystem>())
	{
		mwbs->OnAttack.AddDynamic(this, &AMWPlayerController::NotifyPlayerAttackted);
		mwbs->OnCharacterDied.AddDynamic(this, &AMWPlayerController::OnOtherCharacterDied);
	}
}
