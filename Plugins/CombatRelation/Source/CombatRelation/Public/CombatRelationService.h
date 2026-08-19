// ============================================================================
//  UCombatRelationService : UGameInstanceSubsystem
//  - 陣営関係マトリックスを集中管理するサービス。現在アクティブなUFactionRelationDataAssetを保持
// ============================================================================
#pragma once

#include "CombatRelationTypes.h"
#include "GameplayTagContainer.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CombatRelationService.generated.h"

class UFactionRelationDataAsset;

UCLASS(DisplayName="Combat Relation Service", Config=Game)
class COMBATRELATION_API UCombatRelationService : public UGameInstanceSubsystem
{
    GENERATED_BODY()
public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    UFUNCTION(BlueprintCallable)
    void SetActiveMatrix(UFactionRelationDataAsset* InDataAsset);

    UFUNCTION(BlueprintCallable)
    ECombatRelation Query(const FGameplayTag& SelfFaction, const FGameplayTag& OtherFaction) const;

    UFUNCTION(BlueprintPure)
    bool IsRelationDataSet() const;

    // 設定からマトリックスを再読み込み（エディターでのデバッグに有用）
    UFUNCTION(BlueprintCallable, Category = "Combat Relation")
    void LoadMatrixFromSettings();

private:
    UPROPERTY()
    TObjectPtr<UFactionRelationDataAsset> ActiveMatrix = nullptr;

    // フォールバック設定：Developer Settingsが未設定の場合に使用
    UPROPERTY(Config, EditAnywhere, Category = "Combat Relation", meta = (AllowedClasses = "/Script/CombatRelation.FactionRelationDataAsset"))
    FSoftObjectPath FallbackMatrixPath;
};
