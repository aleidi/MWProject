#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "MWTestSettings.generated.h"

USTRUCT(BlueprintType)
struct FTestStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FKey Key;
};

UCLASS()
class UMWTestSettings : public UGameUserSettings
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable)
	static UMWTestSettings* GetMWTestSettings();

public:
	UPROPERTY(Config, BlueprintReadWrite)
	FTestStruct MyStruct;
};