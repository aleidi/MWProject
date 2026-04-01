#pragma once

// Include Header
#include "Pawn/MWPawnComponent.h"
#include "MWAppearanceComponent.generated.h"

// Forward Declare
struct FStreamableHandle;

// Define

/*
 * @class UMWAppearanceComponent
 * 
 * @brief 
 *
 * @note
 */
UCLASS(Blueprintable, Meta=(BlueprintSpawnableComponent))
class MW_API UMWAppearanceComponent : public UMWPawnComponent
{
	GENERATED_BODY()

public:
	void BeginPlay() override;
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	void OnDefaultAppearanceLoaded(FPrimaryAssetId LoadedAssetId);
	void OnAnimInstanceClassLoaded();

	FPrimaryAssetId AppearancePrimaryAssetId;

	TSharedPtr<FStreamableHandle> AnimInstanceStreamableHandle;
};