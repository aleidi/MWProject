#pragma once

// Include Header
#include "UObject/NoExportTypes.h"
#include "MWGameSingleton.generated.h"

// Forward Declare
class UMWMasterData;
class UMWGameplayData;

// Macro

/*
 * @class UMWGameSingleton
 * 
 * @brief
 *
 * @note
 */
UCLASS()
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
	const UMWMasterData* GetMasterData();

protected:
	bool bInitialized = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMWMasterData> MasterData;
};