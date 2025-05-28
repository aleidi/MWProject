#pragma once

#include "GameplayAbility/MWGameplayAbility.h"
#include "MWBattleCommandBase.generated.h"


UCLASS()
class MW_API UMWBattleCommandBase : public UMWGameplayAbility
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable)
	void K2_OnCommandComplete() { OnCommandComplete(); }

	void OnCommandComplete();
};