#include "GameplayAbility/Ability/MWBattleCommandBase.h"
#include "Gameplay/Battle/MWBattleSystem.h"

void UMWBattleCommandBase::OnCommandComplete()
{
	UMWBattleSystem* mwbs = GetWorld() ? GetWorld()->GetSubsystem<UMWBattleSystem>() : nullptr;
	if (mwbs)
	{
		mwbs->OnActionComplete.Broadcast();
	}
}
