#pragma once

#include "Gameplay/MWPawnComponent.h"
#include "MWHeroComponent.generated.h"

struct FGameplayTag;
struct FInputActionValue;
class UMWInputConfig;

UCLASS(Blueprintable, Meta=(BlueprintSpawnableComponent))
class UMWHeroComponent : public UMWPawnComponent
{
	GENERATED_BODY()

public:
	UMWHeroComponent(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};