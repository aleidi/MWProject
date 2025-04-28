#pragma once

#include "UI/MWUserWidget.h"
#include "MWActionCommand.generated.h"

class UMWCommandBase;

UCLASS()
class MW_API UMWActionCommand : public UMWUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UMWCommandBase> AttackCmd;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UMWCommandBase> SpiritCmd;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UMWCommandBase> ItemCmd;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UMWCommandBase> ChargeCmd;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UMWCommandBase> SpecialCmd;
};