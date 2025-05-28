#pragma once

#include "UI/MWUserWidget.h"
#include "MWActionCommand.generated.h"

class UMWCommandBase;

UCLASS()
class MW_API UMWActionCommand : public UMWUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UMWCommandBase> AttackCmd;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UMWCommandBase> SpiritCmd;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UMWCommandBase> ItemCmd;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UMWCommandBase> ChargeCmd;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UMWCommandBase> SpecialCmd;
};