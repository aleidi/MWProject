#pragma once

// Include Header
#include "UObject/NoExportTypes.h"
#include "MWBattleState.generated.h"

// Forward Declare

// Define

/*
 * @class UMWBattleState
 * 
 * @brief Track battle stats
 *        バトルの状態を管理する
 *
 * @note
 */
UCLASS()
class UMWBattleState : public UObject
{
	GENERATED_BODY()
	
public:
	UMWBattleState(const FObjectInitializer& ObjectInitializer);

public:
	// Start battle timer
	// 戦闘タイマーを開始
	void OnBattleStart();

	// End battle and calculate final stats
	// 戦闘を終了し、最終統計を計算
	void OnBattleEnd();

	void AddEnemyAttendant(AActor* EnemyActor);

	// Current battle attendants enemies number
	// 現在の戦闘参加者の敵の数
	int32 GetCurrentEnemiesNumber() const;

	// Get current battle duration in seconds
	// 現在の戦闘時間（秒）を取得
	float GetBattleDuration() const;

private:

	// Current battle attendants enemies actors
	// 現在の戦闘参加者の敵のアクター
	TArray<TWeakObjectPtr<AActor>> CurrentEnemyActors;

	// Battle start time
	// 戦闘開始時刻
	float BattleStartTime;

	// Is battle active
	// 戦闘中かどうか
	bool bIsBattleActive;

	// Total damage dealt by player party
	// プレイヤーパーティが与えた総ダメージ
	float TotalDamageDealt;

	// Total damage received by player party
	// プレイヤーパーティが受けた総ダメージ
	float TotalDamageReceived;

	// Total healing done
	// 総回復量
	float TotalHealingDone;

	// Number of enemies defeated
	// 倒した敵の数
	int32 EnemiesDefeated;

	// Highest single hit damage
	// 最大単発ダメージ
	float MaxSingleHitDamage;

	// Total number of hits/attacks
	// 総攻撃回数
	int32 TotalHitCount;

	// Number of elemental reactions triggered
	// 元素反応の発動回数
	int32 ElementalReactionCount;

	// Number of critical hits
	// クリティカルヒット数
	int32 CriticalHitCount;

	// Number of combo hits achieved
	// 達成したコンボ数
	int32 MaxComboCount;

	// Number of character switches/swaps
	// キャラクター切り替え回数
	int32 CharacterSwitchCount;

	// Number of ultimate/burst skills used
	// 必殺技使用回数
	int32 UltimateSkillUsedCount;

	// Number of character deaths
	// キャラクター死亡回数
	int32 CharacterDeathCount;

	// Damage dealt per character
	// キャラクター毎の与ダメージ
	TMap<FName, float> DamagePerCharacter;
};