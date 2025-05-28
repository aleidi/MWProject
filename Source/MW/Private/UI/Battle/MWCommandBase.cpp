#include "UI/Battle/MWCommandBase.h"
#include "Components/TextBlock.h"

void UMWCommandBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	if (IsValid(T_Command))
	{
		T_Command->SetText(Text);
	}
}
