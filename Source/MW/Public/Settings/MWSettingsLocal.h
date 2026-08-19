//// Copyright Epic Games, Inc. All Rights Reserved.
//
//#pragma once
//
//#include "MW.h"
//#include "GameFramework/GameUserSettings.h"
//#include "Player/MWLocalPlayer.h"
//#include "Input/MWMappableConfigPair.h"
//#include "MWSettingsLocal.generated.h"
//
//USTRUCT()
//struct FMWScalabilitySnapshot
//{
//	GENERATED_BODY()
//
//	FMWScalabilitySnapshot() {}
//
//	Scalability::FQualityLevels Qualities;
//	bool bActive = false;
//	bool bHasOverrides = false;
//};
//
///**
// * UMWSettingsLocal
// */
//UCLASS()
//class UMWSettingsLocal : public UGameUserSettings
//{
//	GENERATED_BODY()
//
//public:
//
//	UMWSettingsLocal();
//
//	static UMWSettingsLocal* Get();
//
//	//~UObjectインターフェース
//	virtual void BeginDestroy() override;
//	//~UObjectインターフェース終了
//
//	//~UGameUserSettingsインターフェース
//	virtual void SetToDefaults() override;
//	virtual void LoadSettings(bool bForceReload) override;
//	virtual void ConfirmVideoMode() override;
//	virtual float GetEffectiveFrameRateLimit() override;
//	virtual void ResetToCurrentSettings() override;
//	virtual void ApplyNonResolutionSettings() override;
//	virtual int32 GetOverallScalabilityLevel() const override;
//	virtual void SetOverallScalabilityLevel(int32 Value) override;
//	//~UGameUserSettingsインターフェース終了
//
//	
//	//////////////////////////////////////////////////////////////////
//	// フロントエンド状態
//
//public:
//	void SetShouldUseFrontendPerformanceSettings(bool bInFrontEnd);
//protected:
//	bool ShouldUseFrontendPerformanceSettings() const;
//private:
//	bool bInFrontEndForPerformancePurposes = false;
//
//	
//	//////////////////////////////////////////////////////////////////
//	// キーバインド
//public:
//	// 使用するController表現を設定します。単一Platformで複数種類のControllerをサポートできます。
//	// 例えばWin64では、XboxとPlayStationの両Controllerを利用できます。
//	UFUNCTION()
//	void SetControllerPlatform(const FName InControllerPlatform);
//	UFUNCTION()
//	FName GetControllerPlatform() const;
//
//	DECLARE_EVENT_OneParam(UMWSettingsLocal, FInputConfigDelegate, const FLoadedMappableConfigPair& /*Config*/);
//
//	/** 新しい入力設定が登録された際に呼び出すDelegate。 */
//	FInputConfigDelegate OnInputConfigRegistered;
//
//	/** 登録済み入力設定が有効化された際に呼び出すDelegate。 */
//	FInputConfigDelegate OnInputConfigActivated;
//	
//	/** 登録済み入力設定が無効化された際に呼び出すDelegate。 */
//	FInputConfigDelegate OnInputConfigDeactivated;
//	
//	/** 指定した入力設定を登録し、プレイヤーから利用可能にします。 */
//	void RegisterInputConfig(ECommonInputType Type, const UEnhancedInputUserSettings* NewConfig, const bool bIsActive);
//	
//	/** 指定した入力設定を登録解除し、削除した設定数を返します。 */
//	int32 UnregisterInputConfig(const UEnhancedInputUserSettings* ConfigToRemove);
//
//	/** 登録済み入力設定を有効化します。 */
//	void ActivateInputConfig(const UEnhancedInputUserSettings* Config);
//
//	/** 登録済み入力設定を無効化します。 */
//	void DeactivateInputConfig(const UEnhancedInputUserSettings* Config);
//
//	/** 現在登録されている全入力設定を取得します。 */
//	const TArray<FLoadedMappableConfigPair>& GetAllRegisteredInputConfigs() const { return RegisteredInputConfigs; }
//
//	/**
//	 * 入力種別に一致する登録済み入力設定をすべて取得します。
//	 * 
//	 * @param Type 取得する設定種別。ECommonInputType::Countの場合は全設定を対象とします。
//	 * @param OutArray 種別に一致した登録済み入力設定の出力先。
//	 */
//	void GetRegisteredInputConfigsOfType(ECommonInputType Type, OUT TArray<FLoadedMappableConfigPair>& OutArray) const;
//	
//	/**
//	 * 指定したキーボード設定を割り当てます。
//	 * 
//	 * @param MappingName 変更するFPlayerMappableKeyOptionsの名前。
//	 * @param NewKey この項目に割り当てる新しいキー。
//	 */
//	void AddOrUpdateCustomKeyboardBindings(const FName MappingName, const FKey NewKey, UMWLocalPlayer* LocalPlayer);
//
//	const TMap<FName, FKey>& GetCustomPlayerInputConfig() const { return CustomKeyboardConfig; }
//
//private:
//	/**
//	 * プレイヤーが使用中のController名。現在のPlatformで利用可能なUCommonInputBaseControllerData名に対応します。
//	 * GamepadデータはPlatformごとに登録され、<Platform>Game.iniの+ControllerDataに記載されます。
//	 */
//	UPROPERTY(Config)
//	FName ControllerPlatform;
//
//	UPROPERTY(Config)
//	FName ControllerPreset = TEXT("Default");
//
//	/** ユーザーが選択中の入力設定名。 */
//	UPROPERTY(Config)
//	FName InputConfigName = TEXT("Default");
//	
//	/**
//	 * 現在登録されている入力設定の配列。GameFeature Pluginにより設定されます。
//	 * 
//	 * @see UGameFeatureAction_AddInputConfig
//	 */
//	UPROPERTY(VisibleAnywhere)
//	TArray<FLoadedMappableConfigPair> RegisteredInputConfigs;
//	
//	/** プレイヤーが設定したカスタムキーマッピングの配列。デフォルトは空です。 */
//	UPROPERTY(Config)
//	TMap<FName, FKey> CustomKeyboardConfig;
//
//};
