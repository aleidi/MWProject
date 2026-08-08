#pragma once

#include "MW.h"
#include "Engine/Engine.h"
#include "Subsystems/EngineSubsystem.h"
#include "MWEngineSubsystem.generated.h"

//Forward Declare
 class UMWGameSingleton;

UCLASS()
class UMWEngineSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

public:	
	FORCEINLINE UMWGameSingleton* GetGameSingleton() {	return MWGameSingleton;	}

private:
	UPROPERTY()
	TObjectPtr<UMWGameSingleton> MWGameSingleton = nullptr;
};
