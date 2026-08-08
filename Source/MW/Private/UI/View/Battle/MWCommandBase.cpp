#include "UI/View/Battle/MWCommandBase.h"
#include "Components/TextBlock.h"

void UMWCommandBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	if (IsValid(TXT_Command))
	{
		TXT_Command->SetText(Text);
	}
}
