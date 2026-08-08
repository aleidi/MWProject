#pragma once

// Include Header
#include "MW.h"
#include "UI/ViewModel/MWViewModelBase.h"
#include "MWVMCharacterStatus.generated.h"

// Forward Declare
class UTexture2D;

// Define

/*
 * @class UMWVMCharacterStatus
 * 
 * @brief 
 *
 * @note
 */
UCLASS()
class UMWVMCharacterStatus : public UMWViewModelBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess))
	FText PlayerName;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess))
	int32 CurrentHealth = 50;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess))
	int32 MaxHealth = 100;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess))
	int32 Level = 1;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess))
    TObjectPtr<UTexture2D> Portrait;

public:
	void SetPlayerName(const FText& NewName);
	UFUNCTION(BlueprintPure, FieldNotify)
	FText GetPlayerName() const;

	void SetCurrentHealth(int32 NewValue);
	UFUNCTION(BlueprintPure, FieldNotify)
	int32 GetCurrentHealth() const;

	void SetMaxHealth(int32 NewValue);
	UFUNCTION(BlueprintPure, FieldNotify)
	int32 GetMaxHealth() const;

	UFUNCTION(BlueprintPure, FieldNotify)
	FText GetHealthText() const;
	UFUNCTION(BlueprintPure, FieldNotify)
	float GetHealthPercent() const;
	UFUNCTION(BlueprintPure, FieldNotify)
	FLinearColor GetHealthColor() const;

	void SetLevel(int32 NewValue);
	UFUNCTION(BlueprintPure, FieldNotify)
	int32 GetLevel() const;
	UFUNCTION(BlueprintPure, FieldNotify)
	FText GetLevelText() const;

	void SetPortrait(UTexture2D* NewTex);
	UFUNCTION(BlueprintPure, FieldNotify)
	UTexture2D* GetPortrait() const;
};
