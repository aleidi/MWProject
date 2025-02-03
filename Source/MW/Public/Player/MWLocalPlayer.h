//// Copyright Epic Games, Inc. All Rights Reserved.
//
//#pragma once
//
//#include "CoreMinimal.h"
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
//	//~UObject interface
//	virtual void PostInitProperties() override;
//	//~End of UObject interface
//
//	//~UPlayer interface
//	virtual void SwitchController(class APlayerController* PC) override;
//	//~End of UPlayer interface
//
//	//~ULocalPlayer interface
//	virtual bool SpawnPlayActor(const FString& URL, FString& OutError, UWorld* InWorld) override;
//	virtual void InitOnlineSession() override;
//	//~End of ULocalPlayer interface
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
