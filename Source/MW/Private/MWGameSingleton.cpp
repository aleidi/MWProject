#include "MWGameSingleton.h"
#include "Data/MWAssetRegistry.h"
#include "Data/MWDataTableManager.h"
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

	DataTableManager = NewObject<UMWDataTableManager>(this);

	if (DataAsset)
	{
		DataAsset->Initialize();

		DataTableManager->Initialize();
	}

	bInitialized = true;
}

void UMWGameSingleton::PostLoad()
{
	Super::PostLoad();
}
