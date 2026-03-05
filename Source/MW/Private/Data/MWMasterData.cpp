#include "Data/MWMasterData.h"
#include "Input/MWInputConfig.h"

UMWInputConfig* UMWMasterData::FindInputConfig(const FGameplayTag& IMCTag) const
{
	const TObjectPtr<UMWInputConfig>* found = InputConfigs.Find(IMCTag);
	return found ? found->Get() : nullptr;
}
