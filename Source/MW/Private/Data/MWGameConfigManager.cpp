#include "Data/MWGameConfigManager.h"

UMWGameConfigManager::UMWGameConfigManager()
{
	ConfigPath = FPaths::ProjectContentDir() / TEXT("GameConfigs");
}

void UMWGameConfigManager::LoaddAllConfigs()
{
	if (!LoadCharacterConfig())
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load character config"));
	}
}

bool UMWGameConfigManager::LoadCharacterConfig()
{
	return true;
}
