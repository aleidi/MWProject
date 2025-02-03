//// Copyright Epic Games, Inc. All Rights Reserved.
//
//#pragma once
//
//#include "CoreMinimal.h"
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
//	//~UObject interface
//	virtual void BeginDestroy() override;
//	//~End of UObject interface
//
//	//~UGameUserSettings interface
//	virtual void SetToDefaults() override;
//	virtual void LoadSettings(bool bForceReload) override;
//	virtual void ConfirmVideoMode() override;
//	virtual float GetEffectiveFrameRateLimit() override;
//	virtual void ResetToCurrentSettings() override;
//	virtual void ApplyNonResolutionSettings() override;
//	virtual int32 GetOverallScalabilityLevel() const override;
//	virtual void SetOverallScalabilityLevel(int32 Value) override;
//	//~End of UGameUserSettings interface
//
//	
//	//////////////////////////////////////////////////////////////////
//	// Frontend state
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
//	// Keybindings
//public:
//	// Sets the controller representation to use, a single platform might support multiple kinds of controllers.  For
//	// example, Win64 games could be played with both an XBox or Playstation controller.
//	UFUNCTION()
//	void SetControllerPlatform(const FName InControllerPlatform);
//	UFUNCTION()
//	FName GetControllerPlatform() const;
//
//	DECLARE_EVENT_OneParam(UMWSettingsLocal, FInputConfigDelegate, const FLoadedMappableConfigPair& /*Config*/);
//
//	/** Delegate called when a new input config has been registered */
//	FInputConfigDelegate OnInputConfigRegistered;
//
//	/** Delegate called when a registered input config has been activated */
//	FInputConfigDelegate OnInputConfigActivated;
//	
//	/** Delegate called when a registered input config has been deactivate */
//	FInputConfigDelegate OnInputConfigDeactivated;
//	
//	/** Register the given input config with the settings to make it available to the player. */
//	void RegisterInputConfig(ECommonInputType Type, const UEnhancedInputUserSettings* NewConfig, const bool bIsActive);
//	
//	/** Unregister the given input config. Returns the number of configs removed. */
//	int32 UnregisterInputConfig(const UEnhancedInputUserSettings* ConfigToRemove);
//
//	/** Set a registered input config as active */
//	void ActivateInputConfig(const UEnhancedInputUserSettings* Config);
//
//	/** Deactivate a registered config */
//	void DeactivateInputConfig(const UEnhancedInputUserSettings* Config);
//
//	/** Get all currently registered input configs */
//	const TArray<FLoadedMappableConfigPair>& GetAllRegisteredInputConfigs() const { return RegisteredInputConfigs; }
//
//	/**
//	 * Get all registered input configs that match the input type.
//	 * 
//	 * @param Type		The type of config to get, ECommonInputType::Count will include all configs.
//	 * @param OutArray	Array to be populated with the current registered input configs that match the type
//	 */
//	void GetRegisteredInputConfigsOfType(ECommonInputType Type, OUT TArray<FLoadedMappableConfigPair>& OutArray) const;
//	
//	/**
//	 * Maps the given keyboard setting to the 
//	 * 
//	 * @param MappingName	The name of the FPlayerMappableKeyOptions that you would like to change
//	 * @param NewKey		The new key to bind this option to
//	 */
//	void AddOrUpdateCustomKeyboardBindings(const FName MappingName, const FKey NewKey, UMWLocalPlayer* LocalPlayer);
//
//	const TMap<FName, FKey>& GetCustomPlayerInputConfig() const { return CustomKeyboardConfig; }
//
//private:
//	/**
//	 * The name of the controller the player is using.  This is maps to the name of a UCommonInputBaseControllerData
//	 * that is available on this current platform.  The gamepad data are registered per platform, you'll find them
//	 * in <Platform>Game.ini files listed under +ControllerData=...
//	 */
//	UPROPERTY(Config)
//	FName ControllerPlatform;
//
//	UPROPERTY(Config)
//	FName ControllerPreset = TEXT("Default");
//
//	/** The name of the current input config that the user has selected. */
//	UPROPERTY(Config)
//	FName InputConfigName = TEXT("Default");
//	
//	/**
//	 * Array of currently registered input configs. This is populated by game feature plugins
//	 * 
//	 * @see UGameFeatureAction_AddInputConfig
//	 */
//	UPROPERTY(VisibleAnywhere)
//	TArray<FLoadedMappableConfigPair> RegisteredInputConfigs;
//	
//	/** Array of custom key mappings that have been set by the player. Empty by default. */
//	UPROPERTY(Config)
//	TMap<FName, FKey> CustomKeyboardConfig;
//
//};
