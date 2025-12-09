#pragma once

// Include
#include "CoreMinimal.h"

// Forward Declare
class UMWBattleUnit;

// Define
DECLARE_MULTICAST_DELEGATE_OneParam(FOnStatusEffectAdded, const FName&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnStatusEffectRemoved, const FName&);

namespace MWCommandBattle
{

enum class ETriggerTiming : uint8
{
	OnApply,				// When the status effect is applied		// ステータス効果が適用されたとき
	OnRemove,				// When the status effect is removed		// ステータス効果が解除されたとき
	OnPlayerTurnStart,		// At the start of the player's turn		// プレイヤーのターン開始時
	OnPlayerTurnEnd,		// At the end of the player's turn			// プレイヤーのターン終了時
	OnEnemyTurnStart,		// At the start of the enemy's turn			// 敵のターン開始時
	OnEnemyTurnEnd,			// At the end of the enemy's turn			// 敵のターン終了時
	OnReceiveDamage,		// When the unit receives damage			// ユニットがダメージを受けたとき
	OnDealDamage,			// When the unit deals damage				// ユニットがダメージを与えたとき	
	OnBeforeAction,			// Before the unit takes an action			// ユニットが行動する前
	OnAfterAction,			// After the unit takes an action			// ユニットが行動した後
};

/*
 * @class FMWStatusEffect
 *
 * @brief Status effects applied for battle unit.
 *
 * @note
 */
class FMWStatusEffect : public TSharedFromThis<FMWStatusEffect>
{
public:
	FMWStatusEffect():FMWStatusEffect(TEXT(""), 0, false)
	{}

	virtual ~FMWStatusEffect() = default;

	FMWStatusEffect(FName InName, int32 InDuration, bool InStackable)
		: Name(InName), Duration(InDuration), Stackable(InStackable)
	{}

	/** Trigger the status effect 
	 *  ステータス効果を発動する
	 *
	 * @param TargetUnit : The unit the effect is applied to | 効果が適用されるユニット
	 * @param Timing : The timing of the trigger | 発動のタイミング
	 * @param Context : Additional context for the trigger | 発動の追加コンテキスト
	 * @return false if the effect should be removed | 効果を削除する場合はfalse
	 */
	virtual bool OnTrigger(UMWBattleUnit* TargetUnit, ETriggerTiming Timing, void* Context = nullptr);

	// Force to expire the effect
	// 効果を強制的に終了させる
	void ForceExpire();

	FName GetName() const { return Name; }

	FORCEINLINE bool IsStackable() const { return Stackable; }

	bool operator ==(const FMWStatusEffect& Other) const
	{
		return Name == Other.Name;
	}

protected:
	// Called when the effect is applied
	// 効果が適用されたときに呼び出される
	virtual void OnApply(UMWBattleUnit* TargetUnit, void* Context = nullptr) {}

protected:
	FName Name = TEXT("");

	//Duration in turns, -1 means infinite
	//ターンの継続時間, -1は無限
	int32 Duration = 0;		

	//Whether multiple same effect can be applied
	//同じ効果を複数適用できるかどうか
	bool Stackable = false;	

	//When the effect should trigger
	//効果が発動するタイミング
	ETriggerTiming TriggerTiming = ETriggerTiming::OnApply;

	//When the effect duration should update
	//効果の継続時間が更新されるタイミング
	ETriggerTiming DurationUpdateTiming = ETriggerTiming::OnPlayerTurnEnd;
};


/*
 * @class FMWStatusEffectManager
 * 
 * @brief Manages status effects set for battle unit.
 *        ステータス効果を管理する.
 *
 * @note
 */
class FMWStatusEffectManager : public TSharedFromThis<FMWStatusEffectManager>
{
public:
	FMWStatusEffectManager() = delete;

	FMWStatusEffectManager(UMWBattleUnit* InOwnerUnit)
		: OwnerUnit(InOwnerUnit)
	{}

	~FMWStatusEffectManager();

	// Add a new status effect
	// ステータス効果を追加する
	void AddEffect(TSharedPtr<FMWStatusEffect> NewEffect);

	// Remove a status effect
	// ステータス効果を削除する
	void RemoveEffect(TSharedPtr<FMWStatusEffect> EffectToRemove);

	void TriggerEffects(ETriggerTiming Timing, void* Context = nullptr);

	bool IsValid() const { return OwnerUnit != nullptr; }

private:
	TArray<TSharedPtr<FMWStatusEffect>> ActiveEffects;

	TObjectPtr<UMWBattleUnit> OwnerUnit = nullptr;

public:
	FOnStatusEffectAdded OnEffectAdded;

	FOnStatusEffectRemoved OnEffectRemoved;
};

}