//// Copyright Epic Games, Inc. All Rights Reserved.
//
//#include "Settings/MWSettingsLocal.h"
//#include "MWLogChannels.h"
//#include "CommonInputBaseTypes.h"
//#include "CommonInputSubsystem.h"
//#include "Player/MWLocalPlayer.h"
//#include "PlayerMappableInputConfig.h"
//#include "EnhancedInputSubsystems.h"
//#include "CommonInputBaseTypes.h"
//#include "NativeGameplayTags.h"
//#include "ICommonUIModule.h"
//#include "CommonUISettings.h"
//#include "HAL/PlatformFramePacer.h"
//#include "Input/MWMappableConfigPair.h"
//#include "UserSettings/EnhancedInputUserSettings.h"
//#include "EnhancedActionKeyMapping.h"
//
//UMWSettingsLocal::UMWSettingsLocal()
//{
//	SetToDefaults();
//}
//
//UMWSettingsLocal* UMWSettingsLocal::Get()
//{
//	return GEngine ? CastChecked<UMWSettingsLocal>(GEngine->GetGameUserSettings()) : nullptr;
//}
//
//void UMWSettingsLocal::BeginDestroy()
//{
//	Super::BeginDestroy();
//}
//
//void UMWSettingsLocal::SetToDefaults()
//{
//	Super::SetToDefaults();
//}
//
//void UMWSettingsLocal::LoadSettings(bool bForceReload)
//{
//	Super::LoadSettings(bForceReload);
//}
//
//void UMWSettingsLocal::ConfirmVideoMode()
//{
//	Super::ConfirmVideoMode();
//}
//
//float UMWSettingsLocal::GetEffectiveFrameRateLimit()
//{
//	float EffectiveFrameRateLimit = Super::GetEffectiveFrameRateLimit();
//
//	return EffectiveFrameRateLimit;
//}
//
//void UMWSettingsLocal::ResetToCurrentSettings()
//{
//	Super::ResetToCurrentSettings();
//}
//
//void UMWSettingsLocal::ApplyNonResolutionSettings()
//{
//	Super::ApplyNonResolutionSettings();
//}
//
//int32 UMWSettingsLocal::GetOverallScalabilityLevel() const
//{
//	int32 Result = Super::GetOverallScalabilityLevel();
//
//	return Result;
//}
//
//void UMWSettingsLocal::SetOverallScalabilityLevel(int32 Value)
//{
//	Value = FMath::Clamp(Value, 0, 3);
//
//	Super::SetOverallScalabilityLevel(Value);
//}
//
//void UMWSettingsLocal::SetShouldUseFrontendPerformanceSettings(bool bInFrontEnd)
//{
//	bInFrontEndForPerformancePurposes = bInFrontEnd;
//}
//
//bool UMWSettingsLocal::ShouldUseFrontendPerformanceSettings() const
//{
//	return bInFrontEndForPerformancePurposes;
//}
//
//void UMWSettingsLocal::SetControllerPlatform(const FName InControllerPlatform)
//{
//	if (ControllerPlatform != InControllerPlatform)
//	{
//		ControllerPlatform = InControllerPlatform;
//
//		// Apply the change to the common input subsystem so that we refresh any input icons we're using.
//		if (UCommonInputSubsystem* InputSubsystem = UCommonInputSubsystem::Get(GetTypedOuter<ULocalPlayer>()))
//		{
//			InputSubsystem->SetGamepadInputType(ControllerPlatform);
//		}
//	}
//}
//
//FName UMWSettingsLocal::GetControllerPlatform() const
//{
//	return ControllerPlatform;
//}
//
//void UMWSettingsLocal::RegisterInputConfig(ECommonInputType Type, const UEnhancedInputUserSettings* NewConfig, const bool bIsActive)
//{
//	if (NewConfig)
//	{
//		const int32 ExistingConfigIdx = RegisteredInputConfigs.IndexOfByPredicate([&NewConfig](const FLoadedMappableConfigPair& Pair) { return Pair.Config == NewConfig; });
//		if (ExistingConfigIdx == INDEX_NONE)
//		{
//			const int32 NumAdded = RegisteredInputConfigs.Add(FLoadedMappableConfigPair(NewConfig, Type, bIsActive));
//			if (NumAdded != INDEX_NONE)
//			{
//				OnInputConfigRegistered.Broadcast(RegisteredInputConfigs[NumAdded]);
//			}
//		}
//	}
//}
//
//int32 UMWSettingsLocal::UnregisterInputConfig(const UEnhancedInputUserSettings* ConfigToRemove)
//{
//	if (ConfigToRemove)
//	{
//		const int32 Index = RegisteredInputConfigs.IndexOfByPredicate([&ConfigToRemove](const FLoadedMappableConfigPair& Pair) { return Pair.Config == ConfigToRemove; });
//		if (Index != INDEX_NONE)
//		{
//			RegisteredInputConfigs.RemoveAt(Index);
//			return 1;
//		}
//
//	}
//	return INDEX_NONE;
//}
//
//void UMWSettingsLocal::ActivateInputConfig(const UEnhancedInputUserSettings* Config)
//{
//	if (Config)
//	{
//		const int32 ExistingConfigIdx = RegisteredInputConfigs.IndexOfByPredicate([&Config](const FLoadedMappableConfigPair& Pair) { return Pair.Config == Config; });
//		if (ExistingConfigIdx != INDEX_NONE)
//		{
//			RegisteredInputConfigs[ExistingConfigIdx].bIsActive = true;
//			OnInputConfigActivated.Broadcast(RegisteredInputConfigs[ExistingConfigIdx]);
//		}
//	}
//}
//
//void UMWSettingsLocal::DeactivateInputConfig(const UEnhancedInputUserSettings* Config)
//{
//	if (Config)
//	{
//		const int32 ExistingConfigIdx = RegisteredInputConfigs.IndexOfByPredicate([&Config](const FLoadedMappableConfigPair& Pair) { return Pair.Config == Config; });
//		if (ExistingConfigIdx != INDEX_NONE)
//		{
//			RegisteredInputConfigs[ExistingConfigIdx].bIsActive = false;
//			OnInputConfigDeactivated.Broadcast(RegisteredInputConfigs[ExistingConfigIdx]);
//		}
//	}
//}
//
//void UMWSettingsLocal::GetRegisteredInputConfigsOfType(ECommonInputType Type, OUT TArray<FLoadedMappableConfigPair>& OutArray) const
//{
//	OutArray.Empty();
//
//	// If "Count" is passed in then 
//	if (Type == ECommonInputType::Count)
//	{
//		OutArray = RegisteredInputConfigs;
//		return;
//	}
//
//	for (const FLoadedMappableConfigPair& Pair : RegisteredInputConfigs)
//	{
//		if (Pair.Type == Type)
//		{
//			OutArray.Emplace(Pair);
//		}
//	}
//}
//
//void UMWSettingsLocal::AddOrUpdateCustomKeyboardBindings(const FName MappingName, const FKey NewKey, UMWLocalPlayer* LocalPlayer)
//{
//	if (MappingName == NAME_None)
//	{
//		return;
//	}
//
//	if (InputConfigName != TEXT("Custom"))
//	{
//		// Copy Presets.
//		//if (const UEnhancedInputUserSettings* DefaultConfig = GetInputConfigByName(TEXT("Default")))
//		//{
//		//	for (const FEnhancedActionKeyMapping& Mapping : DefaultConfig->GetPlayerMappableKeys())
//		//	{
//		//		// Make sure that the mapping has a valid name, its possible to have an empty name
//		//		// if someone has marked a mapping as "Player Mappabe" but deleted the default field value
//		//		if (Mapping.PlayerMappableOptions.Name != NAME_None)
//		//		{
//		//			CustomKeyboardConfig.Add(Mapping.PlayerMappableOptions.Name, Mapping.Key);
//		//		}
//		//	}
//		//}
//
//		InputConfigName = TEXT("Custom");
//	}
//
//	if (FKey* ExistingMapping = CustomKeyboardConfig.Find(MappingName))
//	{
//		// Change the key to the new one
//		CustomKeyboardConfig[MappingName] = NewKey;
//	}
//	else
//	{
//		CustomKeyboardConfig.Add(MappingName, NewKey);
//	}
//
//	// Tell the enhanced input subsystem for this local player that we should remap some input! Woo
//	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
//	{
//		Subsystem->GetUserSettings->(MappingName, NewKey);
//	}
//}
