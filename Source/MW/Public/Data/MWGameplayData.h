#pragma once

#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "MWGameplayData.generated.h"

// 前方宣言
class AMWCharacter;
class UC3DCameraComponent;
class UC3DCameraModeDataAsset;
class UGameplayEffect;
class UMWInputConfig;
class UMWGameplayData;
class UMWUIConfigData;
/*
 * @class UMWGameplayData
 *
 * @brief ゲームプレイ関連の定数データです。
 *        ゲームプレイに関連する定数データ。
 *
 */
UCLASS(BlueprintType, Const, Meta = (DisplayName = "MW Gameplay Data", ShortTooltip = "Data asset containing gaameplay data."))
class UMWGameplayData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Default")
	TSubclassOf<AMWCharacter> DefaultPawn;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Default")
	//TObjectPtr<UMWCharacterData> DefaultPawnData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Default")
	TSubclassOf<UAnimInstance> DummyAnimClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Default")
	TObjectPtr<USkeletalMesh> DummyMesh;

	/**
	 * IMCのGameplayTagをキーとする全入力設定のMapです。
	 * 実行時の設定取得にはFindInputConfig()を使用します。
	 *
	 * 推奨するタグ命名規則：
	 *   IMC.TPDefault, IMC.Basic, IMC.BattleCommand, IMC.BattleCommand.Attack, etc.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TMap<FGameplayTag, TObjectPtr<UMWInputConfig>> InputConfigs;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UMWUIConfigData> UIConfig;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
	TArray<TObjectPtr<UC3DCameraModeDataAsset>> DefaultCameraModesAssets;

public:
	/**
	 * IMCのGameplayTagからInputConfigを検索します。
	 * @return 見つからない場合はnullptr。
	 */
	UFUNCTION(BlueprintPure, Category = "Input")
	UMWInputConfig* FindInputConfig(const FGameplayTag& IMCTag) const;

public:
	// 動的タグの追加と削除に使用するGameplayEffect。
	UPROPERTY(EditDefaultsOnly, Category = "Battle|Default Gameplay Effects")
	TSubclassOf<UGameplayEffect> GE_DynamicTag;

	UPROPERTY(EditDefaultsOnly, Category = "Battle|Default Gameplay Effects")
	TSubclassOf<UGameplayEffect> GE_InputCharging;

	UPROPERTY(EditDefaultsOnly, Category = "Battle|Default Gameplay Effects")
	TSubclassOf<UGameplayEffect> GE_Damage;
};