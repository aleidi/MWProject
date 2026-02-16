
// ============================================================================
//  UFactionRelationDataAsset : Faction 矩阵数据
//  - SelfFaction -> (OtherFaction -> Relation)
// ============================================================================
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "CombatRelationTypes.h"
#include "FactionRelationDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FFactionRelationRow
{
    GENERATED_BODY()

    // OtherFaction -> Relation
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TMap<FGameplayTag, ECombatRelation> Relations;
};

UCLASS(BlueprintType)
class COMBATRELATION_API UFactionRelationDataAsset : public UDataAsset
{
    GENERATED_BODY()

public:
    // SelfFaction -> Row
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TMap<FGameplayTag, FFactionRelationRow> Table;

    UFUNCTION(BlueprintCallable)
    ECombatRelation Query(const FGameplayTag& SelfFaction, const FGameplayTag& OtherFaction) const;

protected:
    void InitializeDefaultFactions();
};
