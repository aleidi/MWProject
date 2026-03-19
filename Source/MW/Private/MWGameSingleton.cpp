#include "MWGameSingleton.h"
#include "System/MWAssetManager.h"

UMWGameSingleton::UMWGameSingleton(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UMWGameSingleton* UMWGameSingleton::Get()
{
	if (GEngine)
	{
		return Cast<UMWGameSingleton>(GEngine->GameSingleton);
	}

	return nullptr;
}

void UMWGameSingleton::Initialize()
{
	if (bInitialized)
	{
		return;
	}

	bInitialized = true;
}

void UMWGameSingleton::PostLoad()
{
	Super::PostLoad();
}
