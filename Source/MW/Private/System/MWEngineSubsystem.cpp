#include "System/MWEngineSubsystem.h"
#include "MWGameSingleton.h"
#include "MWLogChannels.h"


void UMWEngineSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (IsRunningCookCommandlet())
	{
		return;
	}

	if (GEngine->GameSingleton == nullptr && GEngine->GameSingletonClassName.ToString().Len() > 0)
	{
		UClass* singletonClass = LoadClass<UObject>(nullptr, *GEngine->GameSingletonClassName.ToString());

		if (singletonClass)
		{
			GEngine->GameSingleton = NewObject<UObject>(this, singletonClass);

			if (MWGameSingleton = Cast<UMWGameSingleton>(GEngine->GameSingleton))
			{
				MWGameSingleton->Initialize();
			}
			else
			{
				MW_LOG_ERROR(TEXT("Engine config value GameSingleton '%s' is not UMWGameSingleton."), *GEngine->GameSingletonClassName.ToString());
			}
		}
		else
		{
			MW_LOG_ERROR(TEXT("Engine config value GameSingletonClassName '%s' is not a valid class name."), *GEngine->GameSingletonClassName.ToString());
		}
	}
}

void UMWEngineSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

