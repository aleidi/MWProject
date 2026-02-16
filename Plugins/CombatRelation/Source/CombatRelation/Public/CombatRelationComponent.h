// ============================================================================
//  UCombatRelationComponent : UPawnComponent (Tick Cleanup)
//
//  Overview
//  - Stores identity: TeamId / Faction / PartyId
//  - Manages instance-level overrides (Actor/Faction)
//  - Unified interface: GetRelationTo / IsValidTarget
//  - Relation priority: ActorOverride > Party > Team > FactionOverride > Service.Matrix
//  - Expiration cleanup: Tick (configurable interval CleanupTickInterval, default 0.5s)
//    * Avoids per-frame O(N) waste; batch cleanup once interval is reached
// ============================================================================
#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "GameplayTagContainer.h"
#include "CombatRelationTypes.h"
#include "CombatRelationComponent.generated.h"

class UCombatRelationService;

USTRUCT()
struct FRelationOverrideEntry
{
	GENERATED_BODY()

	UPROPERTY()
	TWeakObjectPtr<const AActor> Target;

	UPROPERTY()
	ECombatRelation Relation = ECombatRelation::Neutral;

	UPROPERTY()
	float ExpireTime = -1.f; // <0: never
};

USTRUCT()
struct FFactionOverrideEntry
{
	GENERATED_BODY()

	UPROPERTY()
	ECombatRelation Relation = ECombatRelation::Neutral;

	UPROPERTY()
	float ExpireTime = -1.f; // <0: never
};

UCLASS(ClassGroup=(Combat), meta=(BlueprintSpawnableComponent))
class COMBATRELATION_API UCombatRelationComponent : public UPawnComponent
{
	GENERATED_BODY()

public:
	UCombatRelationComponent(const FObjectInitializer& ObjectInitializer);

	// == Identity ==
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Relation|Base")
	uint8 TeamId = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Relation|Base")
	FGameplayTag Faction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Relation|Base")
	int32 PartyId = 0;

	// == Performance ==
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Relation|Performance")
	float CleanupTickInterval = 0.5f;

	// == Overrides API ==
	UFUNCTION(BlueprintCallable, Category = "Relation|Override")
	void SetActorOverride(AActor* Target, ECombatRelation Relation, float DurationSeconds = -1.f);

	UFUNCTION(BlueprintCallable, Category = "Relation|Override")
	void ClearActorOverride(AActor* Target);

	UFUNCTION(BlueprintCallable, Category = "Relation|Override")
	void SetFactionOverride(FGameplayTag OtherFaction, ECombatRelation Relation, float DurationSeconds = -1.f);

	UFUNCTION(BlueprintCallable, Category = "Relation|Override")
	void ClearFactionOverride(FGameplayTag OtherFaction);

	UFUNCTION(BlueprintCallable, Category = "Relation|Override")
	void ClearAllOverrides();

	// == Query API ==
	UFUNCTION(BlueprintCallable, Category = "Relation")
	ECombatRelation GetRelationTo(const AActor* Other) const;

	UFUNCTION(BlueprintCallable, Category = "Relation")
	bool IsValidTarget(const AActor* Other, ETargetPolicy Policy) const;

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	// Storage
	UPROPERTY()
	TArray<FRelationOverrideEntry> ActorOverrides;

	UPROPERTY()
	TMap<FGameplayTag, FFactionOverrideEntry> FactionOverrides;

	// Tick self-throttle
	float LastCleanupTime = -1.f;

private:
	UCombatRelationService* GetService() const;

	void CleanupExpiredOverrides_Internal();
};
