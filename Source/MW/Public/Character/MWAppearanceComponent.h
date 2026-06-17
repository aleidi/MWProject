#pragma once

// Include Header
#include "Pawn/MWPawnComponent.h"
#include "MWAppearanceComponent.generated.h"

// Forward Declare
class UMWCharacterPrimaryData;

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
	void ApplyAppearance(UMWCharacterPrimaryData* PrimaryData);

	int32 CachedCharacterDataId = INDEX_NONE;
};