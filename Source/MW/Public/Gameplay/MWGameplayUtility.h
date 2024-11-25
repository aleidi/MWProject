#pragma once

#include "Kismet/GameplayStatics.h"
#include "MWGameplayUtility.generated.h"

class UMWBattleSystem;

UCLASS()
class MW_API UWMGameplayUtility : public UGameplayStatics
{
	GENERATED_BODY()

public:
	/* Find selectable targets in front of the center within distance. */
	UFUNCTION(BlueprintCallable, Category="Gameplay")
	static void SearchSelectableTargets(APlayerController* PC, TArray<FMWActorInfo>& FindTargets);

	/* Quick to get Battle Subsystem. */
	UFUNCTION(BlueprintCallable, Category="Gameplay", meta=(WorldContext = "WorldContextObject"))
	static UMWBattleSystem* GetBattleSubSystem(const UObject* WorldContextObject);

	/* Quick to get Battle Subsystem. */
	UFUNCTION(BlueprintCallable, Category="Gameplay", meta=(WorldContext = "WorldContextObject"))
	static UMWGameplaySubsystem* GetGameplaySubsystem(const UObject* WorldContextObject);
};