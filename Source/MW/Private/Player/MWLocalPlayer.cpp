//// Copyright Epic Games, Inc. All Rights Reserved.
//
//#include "Player/MWLocalPlayer.h"
//#include "Settings/MWSettingsLocal.h"
//#include "Input/MWInputConfig.h"
//#include "InputMappingContext.h"
//#include "GameFramework/PlayerController.h"
//
//UMWLocalPlayer::UMWLocalPlayer()
//{
//}
//
//void UMWLocalPlayer::PostInitProperties()
//{
//	Super::PostInitProperties();
//}
//
//void UMWLocalPlayer::SwitchController(class APlayerController* PC)
//{
//	Super::SwitchController(PC);
//
//	OnPlayerControllerChanged(PlayerController);
//}
//
//bool UMWLocalPlayer::SpawnPlayActor(const FString& URL, FString& OutError, UWorld* InWorld)
//{
//	const bool bResult = Super::SpawnPlayActor(URL, OutError, InWorld);
//
//	OnPlayerControllerChanged(PlayerController);
//
//	return bResult;
//}
//
//void UMWLocalPlayer::InitOnlineSession()
//{
//	OnPlayerControllerChanged(PlayerController);
//
//	Super::InitOnlineSession();
//}
//
//void UMWLocalPlayer::OnPlayerControllerChanged(APlayerController* NewController)
//{
//}
//
//
//UMWSettingsLocal* UMWLocalPlayer::GetLocalSettings() const
//{
//	return UMWSettingsLocal::Get();
//}
