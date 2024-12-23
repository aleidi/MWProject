#include "Interface/MWCharacterInterface.h"
#include "Character/MWTargetSelector.h"

TWeakPtr<FMWTargetSelector> IMWCharacterInterface::GetTargetSelector() const
{
	return TWeakPtr<FMWTargetSelector>();
}
