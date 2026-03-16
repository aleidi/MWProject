#pragma once

// Include Header
#include "UObject/NoExportTypes.h"
#include "MWGameSingleton.generated.h"

// Forward Declare
class UMWMasterData;
class UMWAssetRegistry;
class UMWGameplayData;
class UMWDataTableManager;

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
	const UMWMasterData* GetMasterData() { return MasterData; }

	UMWAssetRegistry* GetDataAsset() { return DataAsset; }

	FORCEINLINE UMWGameplayData* GetGameplayData() { return GameplayData; }

	FORCEINLINE	UMWDataTableManager* GetDataTableManager() { return DataTableManager; }
protected:
	bool bInitialized = false;

	UPROPERTY(EditAnywhere, Category = "MWMasterData")
	TObjectPtr<UMWMasterData> MasterData;

	UPROPERTY(EditAnywhere, Category = "MWAssetRegistry")
	TObjectPtr<UMWAssetRegistry> DataAsset;

	UPROPERTY(EditAnywhere, Category = "MWGameplayData")
	TObjectPtr<UMWGameplayData> GameplayData;

	UPROPERTY()
	TObjectPtr<UMWDataTableManager> DataTableManager;
};