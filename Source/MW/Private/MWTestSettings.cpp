#include "MWTestSettings.h"

UMWTestSettings* UMWTestSettings::GetMWTestSettings()
{
	return Cast<UMWTestSettings>(GetGameUserSettings());
}
