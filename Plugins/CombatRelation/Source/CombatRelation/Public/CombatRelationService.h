// ============================================================================
//  UCombatRelationService : UGameInstanceSubsystem
//  - Centralized faction matrix service; holds the currently active UFactionRelationDataAsset
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

    // Reload matrix from settings (useful for debugging in editor)
    UFUNCTION(BlueprintCallable, Category = "Combat Relation")
    void LoadMatrixFromSettings();

private:
    UPROPERTY()
    TObjectPtr<UFactionRelationDataAsset> ActiveMatrix = nullptr;

    // Fallback configuration: used when Developer Settings is not set
    UPROPERTY(Config, EditAnywhere, Category = "Combat Relation", meta = (AllowedClasses = "/Script/CombatRelation.FactionRelationDataAsset"))
    FSoftObjectPath FallbackMatrixPath;
};
