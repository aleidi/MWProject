#include "UI/ViewModel/Battle/MWVMCharacterSkill.h"

void UMWVMCharacterSkill::SetSkillName(const FText& NewValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(SkillName, NewValue);
}

FText UMWVMCharacterSkill::GetSkillName() const
{
	return SkillName;
}

void UMWVMCharacterSkill::SetSkillIcon(UTexture2D* NewValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(SkillIcon, NewValue);
}

UTexture2D* UMWVMCharacterSkill::GetSkillIcon() const
{
	return SkillIcon;
}

void UMWVMCharacterSkill::SetRemainUse(int32 NewValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(RemainUse, NewValue);
}

int32 UMWVMCharacterSkill::GetRemainUse() const
{
	return RemainUse;
}

void UMWVMCharacterSkill::SetCurrentRecoveryAmount(float NewValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(CurrentRecoveryAmount, NewValue);
}

float UMWVMCharacterSkill::GetCurrentRecoveryAmount() const
{
	return CurrentRecoveryAmount;
}

void UMWVMCharacterSkill::SetMaxRecoveryAmount(float NewValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(MaxRecoveryAmount, NewValue);
}

float UMWVMCharacterSkill::GetMaxRecoveryAmount() const
{
	return MaxRecoveryAmount;
}

float UMWVMCharacterSkill::GetRecoveryPercent() const
{
	if (MaxRecoveryAmount <= 0.0f)
	{
		return 0.0f;
	}

	return CurrentRecoveryAmount / MaxRecoveryAmount;
}
