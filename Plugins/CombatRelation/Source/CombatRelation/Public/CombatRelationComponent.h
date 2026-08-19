// ============================================================================
//  UCombatRelationComponent : UPawnComponent（Tickによるクリーンアップ）
//
//  概要
//  - 識別情報を保持：TeamId / Faction / PartyId
//  - インスタンス単位のオーバーライドを管理（アクター／陣営）
//  - 統一インターフェース：GetRelationTo / IsValidTarget
//  - 関係の優先順位：ActorOverride > Party > Team > FactionOverride > Service.Matrix
//  - 期限切れのクリーンアップ：Tick（設定可能な間隔 CleanupTickInterval、デフォルト0.5秒）
//    * フレームごとのO(N)処理を避け、間隔到達時にまとめてクリーンアップ
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
	float ExpireTime = -1.f; // <0: 無期限
};

USTRUCT()
struct FFactionOverrideEntry
{
	GENERATED_BODY()

	UPROPERTY()
	ECombatRelation Relation = ECombatRelation::Neutral;

	UPROPERTY()
	float ExpireTime = -1.f; // <0: 無期限
};

UCLASS(ClassGroup=(Combat), meta=(BlueprintSpawnableComponent))
class COMBATRELATION_API UCombatRelationComponent : public UPawnComponent
{
	GENERATED_BODY()

public:
	UCombatRelationComponent(const FObjectInitializer& ObjectInitializer);

	// == 識別情報 ==
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Relation|Base")
	uint8 TeamId = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Relation|Base")
	FGameplayTag Faction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Relation|Base")
	int32 PartyId = 0;

	// == パフォーマンス ==
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Relation|Performance")
	float CleanupTickInterval = 0.5f;

	// == オーバーライドAPI ==
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

	// == 問い合わせAPI ==
	UFUNCTION(BlueprintCallable, Category = "Relation")
	ECombatRelation GetRelationTo(const AActor* Other) const;

	UFUNCTION(BlueprintCallable, Category = "Relation")
	bool IsValidTarget(const AActor* Other, ETargetPolicy Policy) const;

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	// 格納
	UPROPERTY()
	TArray<FRelationOverrideEntry> ActorOverrides;

	UPROPERTY()
	TMap<FGameplayTag, FFactionOverrideEntry> FactionOverrides;

	// Tickの自己抑制
	float LastCleanupTime = -1.f;

private:
	UCombatRelationService* GetService() const;

	void CleanupExpiredOverrides_Internal();
};
