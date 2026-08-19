#pragma once

// ヘッダーをインクルード
#include "Pawn/MWPawnComponent.h"
#include "MWAppearanceComponent.generated.h"

// 前方宣言
class UMWCharacterAsset;

/*
 * @class UMWAppearanceComponent
 *
 * @brief キャラクター生成時にUMWCharacterPrimaryDataに基づいて外観とAnimInstanceを設定します。
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