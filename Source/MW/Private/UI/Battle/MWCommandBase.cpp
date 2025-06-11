#include "UI/Battle/MWCommandBase.h"
#include "Components/TextBlock.h"

void UMWCommandBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	if (IsValid(Txt_Command))
	{
		Txt_Command->SetText(Text);
	}
}
