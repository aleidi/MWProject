#pragma once

#include "UI/MWUserWidget.h"
#include "MWCommandBase.generated.h"

class UTextBlock;

UCLASS()
class MW_API UMWCommandBase : public UMWUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Command;
};