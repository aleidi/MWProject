// ============================================================================
//  UCombatRelationSettings : UDeveloperSettings
//  - Provides editor-friendly faction relation configuration interface
// ============================================================================
#pragma once

#include "Engine/DeveloperSettings.h"
#include "CombatRelationSettings.generated.h"

class UFactionRelationDataAsset;

UCLASS(Config=Game, DefaultConfig, meta = (DisplayName="Combat Relation Settings"))
class COMBATRELATION_API UCombatRelationSettings : public UDeveloperSettings
{
    GENERATED_BODY()

public:
    // Default faction relation data asset
    UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Default", meta = (AllowedClasses = "/Script/CombatRelation.FactionRelationDataAsset"))
    TSoftObjectPtr<UFactionRelationDataAsset> DefaultFactionRelationAsset;

    virtual FName GetCategoryName() const override { return TEXT("Plugins"); }

#if WITH_EDITOR
    // Listen for property changes in the editor
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

    // Add PostInitProperties to ensure the asset is properly referenced
    virtual void PostInitProperties() override;
};