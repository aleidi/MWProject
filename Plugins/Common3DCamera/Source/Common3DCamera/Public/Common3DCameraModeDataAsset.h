#pragma once

#include "Common3DTypes.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"

#include "Common3DCameraModeDataAsset.generated.h"

UCLASS()
class COMMON3DCAMERA_API UC3DCameraModeDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	//~ UObjectインターフェース開始
	//virtual void Serialize(FArchive& Ar) override;
	//~ UObjectインターフェース終了

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	FGameplayTag ModeTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	FC3DCameraMode ModeSettings;
};
