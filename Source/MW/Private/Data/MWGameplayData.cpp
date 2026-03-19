#include "Data/MWGameplayData.h"

UMWInputConfig* UMWGameplayData::FindInputConfig(const FGameplayTag& IMCTag) const
{
	const TObjectPtr<UMWInputConfig>* found = InputConfigs.Find(IMCTag);

	return found ? found->Get() : nullptr;
}
