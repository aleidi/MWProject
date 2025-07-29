#pragma once

// Include Header
#include "Engine/GameInstance.h"
#include "MWGameInstance.generated.h"

// Forward Declare

// Macro

/*
 * @class UMWGameInstance
 * 
 * @brief
 *
 * @note
 */
UCLASS()
class MW_API UMWGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, meta=(WorldContext="WorldContextObject"))
	static UMWGameInstance* Get(const UObject* WorldContextObject)
	{
		UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
		return World ? World->GetGameInstance<UMWGameInstance>() : nullptr;
	}
};