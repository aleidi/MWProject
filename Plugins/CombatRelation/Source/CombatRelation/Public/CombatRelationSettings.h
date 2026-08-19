// ============================================================================
//  UCombatRelationSettings : UDeveloperSettings
//  - エディターで扱いやすい陣営関係設定インターフェースを提供
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
    // デフォルトの陣営関係データアセット
    UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Default", meta = (AllowedClasses = "/Script/CombatRelation.FactionRelationDataAsset"))
    TSoftObjectPtr<UFactionRelationDataAsset> DefaultFactionRelationAsset;

    virtual FName GetCategoryName() const override { return TEXT("Plugins"); }

#if WITH_EDITOR
    // エディターでのプロパティ変更を監視
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

    // アセットが正しく参照されるようPostInitPropertiesを追加
    virtual void PostInitProperties() override;
};