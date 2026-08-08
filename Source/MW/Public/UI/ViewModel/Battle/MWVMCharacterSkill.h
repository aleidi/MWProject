#pragma once

#include "MW.h"
#include "UI/ViewModel/MWViewModelBase.h"
#include "MWVMCharacterSkill.generated.h"

UCLASS()
class UMWVMCharacterSkill : public UMWViewModelBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess))
    FText SkillName;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess))
    UTexture2D* SkillIcon;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess))
    int32 RemainUse;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess))
	float CurrentRecoveryAmount;

	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess))
	float MaxRecoveryAmount;

public:
	void SetSkillName(const FText& NewValue);
	UFUNCTION(BlueprintPure, FieldNotify)
	FText GetSkillName() const;

	void SetSkillIcon(UTexture2D* NewValue);
	UFUNCTION(BlueprintPure, FieldNotify)
	UTexture2D* GetSkillIcon() const;

	void SetRemainUse(int32 NewValue);
	UFUNCTION(BlueprintPure, FieldNotify)
	int32 GetRemainUse() const;

	void SetCurrentRecoveryAmount(float NewValue);
	UFUNCTION(BlueprintPure, FieldNotify)
	float GetCurrentRecoveryAmount() const;

	void SetMaxRecoveryAmount(float NewValue);
	UFUNCTION(BlueprintPure, FieldNotify)
	float GetMaxRecoveryAmount() const;

	UFUNCTION(BlueprintPure, FieldNotify)
	float GetRecoveryPercent() const;
};
