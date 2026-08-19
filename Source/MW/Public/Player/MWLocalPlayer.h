//// Copyright Epic Games, Inc. All Rights Reserved.
//
//#pragma once
//
//#include "MW.h"
//#include "Engine/LocalPlayer.h"
//#include "MWLocalPlayer.generated.h"
//
//class UMWSettingsLocal;
//class UInputMappingContext;
//
///**
// * UMWLocalPlayer
// */
//UCLASS()
//class MW_API UMWLocalPlayer : public ULocalPlayer
//{
//	GENERATED_BODY()
//
//public:
//
//	UMWLocalPlayer();
//
//	//~UObjectインターフェース
//	virtual void PostInitProperties() override;
//	//~UObjectインターフェース終了
//
//	//~UPlayerインターフェース
//	virtual void SwitchController(class APlayerController* PC) override;
//	//~UPlayerインターフェース終了
//
//	//~ULocalPlayerインターフェース
//	virtual bool SpawnPlayActor(const FString& URL, FString& OutError, UWorld* InWorld) override;
//	virtual void InitOnlineSession() override;
//	//~ULocalPlayerインターフェース終了
//
//public:
//	UFUNCTION()
//	UMWSettingsLocal* GetLocalSettings() const;
//
//private:
//	void OnPlayerControllerChanged(APlayerController* NewController);
//
//private:
//	UPROPERTY(Transient)
//	mutable const UInputMappingContext* InputMappingContext;
//
//	UPROPERTY()
//	TWeakObjectPtr<APlayerController> LastBoundPC;
//};
