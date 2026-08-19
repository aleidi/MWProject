#pragma once

#include "Kismet/GameplayStatics.h"
#include "MWGameplayUtility.generated.h"

class UMWBattleSystem;

UCLASS()
class MW_API UWMGameplayUtility : public UGameplayStatics
{
	GENERATED_BODY()

public:
	/* 中心前方の指定距離内から選択可能なターゲットを検索します。 */
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	static void SearchSelectableTargets(const APlayerController* PC, TArray<FMWFoundActorInfo>& FindTargets, const TArray<AActor*>& ActorsToIgnore);

	/* Blueprintから設定を保存します。 */
	UFUNCTION(BlueprintCallable, Category = "Gameplay", meta = (WorldContext = "WorldContextObject"))
	static void SaveConfig(UObject* WorldContextObject);

	/* Blueprintから設定をロードします。 */
	UFUNCTION(BlueprintCallable, Category = "Gameplay", meta = (WorldContext = "WorldContextObject"))
	static void LoadConfig(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "GameplayAbility")
	static void CastAbility(AActor* Actor, const FGameplayTag& AbilityTag);
};