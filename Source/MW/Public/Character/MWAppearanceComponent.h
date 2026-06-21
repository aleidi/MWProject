#pragma once

// Include Header
#include "Pawn/MWPawnComponent.h"
#include "MWAppearanceComponent.generated.h"

// Forward Declare
class UMWCharacterAsset;

/*
 * @class UMWAppearanceComponent
 *
 * @brief 在 character spawn 时根据 UMWCharacterPrimaryData 设置外观与 AnimInstance。
 */
UCLASS(Blueprintable, Meta=(BlueprintSpawnableComponent))
class MW_API UMWAppearanceComponent : public UMWPawnComponent
{
	GENERATED_BODY()

public:
	void BeginPlay() override;
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	void HandleCharacterAssetReady(UMWCharacterAsset* Asset);
	void ApplyAppearance(UMWCharacterAsset* Asset);

	FDelegateHandle CharacterAssetReadyHandle;
};