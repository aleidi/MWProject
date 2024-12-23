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
	static void SearchSelectableTargets(APlayerController* PC, TArray<FMWFoundActorInfo>& FindTargets, const TArray<AActor*>& ActorsToIgnore);

	/* Quick to get Battle Subsystem. */
	UFUNCTION(BlueprintCallable, Category="Gameplay", meta=(WorldContext = "WorldContextObject"))
	static UMWBattleSystem* GetBattleSubSystem(const UObject* WorldContextObject);

	/* Quick to get Battle Subsystem. */
	UFUNCTION(BlueprintCallable, Category="Gameplay", meta=(WorldContext = "WorldContextObject"))
	static UMWGameplaySubsystem* GetGameplaySubsystem(const UObject* WorldContextObject);

	/* Access to save config in blueprint.*/
	UFUNCTION(BlueprintCallable, Category="Gameplay", meta = (WorldContext = "WorldContextObject"))
	static void SaveConfig(UObject* WorldContextObject);

	/* Access to load config in blueprint.*/
	UFUNCTION(BlueprintCallable, Category="Gameplay", meta = (WorldContext = "WorldContextObject"))
	static void LoadConfig(UObject* WorldContextObject);
};