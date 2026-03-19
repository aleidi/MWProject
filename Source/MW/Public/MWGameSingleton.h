#pragma once

// Include Header
#include "UObject/NoExportTypes.h"
#include "MWGameSingleton.generated.h"

// Forward Declare
class UMWGameplayData;

// Define
#define MWSINGLETON (UMWGameSingleton::Get())

/*
 * @class UMWGameSingleton
 * 
 * @brief
 *
 * @note
 */
UCLASS(BlueprintType, Blueprintable)
class UMWGameSingleton : public UObject
{
	GENERATED_BODY()

public:
	UMWGameSingleton(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintPure, meta = (DisplayName="GetGameSingleton"))
	static UMWGameSingleton* Get();

	virtual void Initialize();

	virtual void PostLoad() override;

	UFUNCTION(BlueprintPure)
	const UMWGameplayData* GetGameplayData() const { return GameplayData; }

	FORCEINLINE UMWGameplayData* GetGameplayData() { return GameplayData; }

protected:
	bool bInitialized = false;

	UPROPERTY(EditAnywhere, Category = "MWGameplayData")
	TObjectPtr<UMWGameplayData> GameplayData;
};