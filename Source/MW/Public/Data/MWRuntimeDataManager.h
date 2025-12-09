#pragma once

// Include Header
#include "UObject/NoExportTypes.h"
#include "MWRuntimeDataManager.generated.h"

// Forward Declare

// Define
USTRUCT(BlueprintType)
struct FMWPlayerStatus
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	int32 Health = 0;

	UPROPERTY(BlueprintReadWrite)
	int32 Spirit = 0;

	UPROPERTY(BlueprintReadWrite)
	int32 Experience = 0;

	UPROPERTY(BlueprintReadWrite)
	int32 Level = 0;
};

/*
 * @class UMWRuntimeDataManager
 * 
 * @brief
 *
 * @note
 */
UCLASS()
class MW_API UMWRuntimeDataManager : public UObject
{
	GENERATED_BODY()

public:
	void Initialize() {}
	void Deinitialize() {}

	UFUNCTION(BlueprintPure, Category = "Manager", meta = (WorldContext="WorldContext", DisplayName="GetRuntimeDataManager"))
	static UMWRuntimeDataManager* Get(const UObject* WorldContext);
};