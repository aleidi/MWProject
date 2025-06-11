#pragma once

#include "UI/MWUserWidget.h"
#include "MWCommandBase.generated.h"

class UTextBlock;

UCLASS()
class MW_API UMWCommandBase : public UMWUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, DisplayName = "Command"))
	TObjectPtr<UTextBlock> Txt_Command;

	/* The real content of Command. */
	UPROPERTY(EditAnywhere)
	FText Text;
};