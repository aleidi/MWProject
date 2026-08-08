#include "UI/ViewModel/Battle/MWVMCharacterStatus.h"

void UMWVMCharacterStatus::SetPlayerName(const FText& NewName)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(PlayerName, NewName))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetPlayerName);
	}
}

FText UMWVMCharacterStatus::GetPlayerName() const
{
	return PlayerName;
}

void UMWVMCharacterStatus::SetCurrentHealth(int32 NewValue)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(CurrentHealth, NewValue))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetHealthText);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetHealthPercent);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetHealthColor);
	}
}

int32 UMWVMCharacterStatus::GetCurrentHealth() const
{
	return CurrentHealth;
}

void UMWVMCharacterStatus::SetMaxHealth(int32 NewValue)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(MaxHealth, NewValue))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetHealthText);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetHealthPercent);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetHealthColor);
	}
}

int32 UMWVMCharacterStatus::GetMaxHealth() const
{
	return MaxHealth;
}

FText UMWVMCharacterStatus::GetHealthText() const
{
	return FText::Format(
		NSLOCTEXT("UI", "HPFormat", "{0} / {1}"),
		FText::AsNumber(CurrentHealth),
		FText::AsNumber(MaxHealth));
}

float UMWVMCharacterStatus::GetHealthPercent() const
{
	if (MaxHealth <= 0)
	{
		return 0.0f;
	}

	return static_cast<float>(CurrentHealth) / static_cast<float>(MaxHealth);
}

FLinearColor UMWVMCharacterStatus::GetHealthColor() const
{
	float Percent = GetHealthPercent();

	if (Percent > 0.5f)
	{
		return FLinearColor::Green;
	}
	else if (Percent > 0.25f)
	{
		return FLinearColor::Yellow;
	}
	else
	{
		return FLinearColor::Red;
	}
}

void UMWVMCharacterStatus::SetLevel(int32 NewValue)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(Level, NewValue))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetLevelText);
	}
}

int32 UMWVMCharacterStatus::GetLevel() const
{
	return Level;
}

FText UMWVMCharacterStatus::GetLevelText() const
{
	return FText::Format(
		NSLOCTEXT("UI", "LevelFormat", "Lv. {0}"),
		FText::AsNumber(Level));
}

void UMWVMCharacterStatus::SetPortrait(UTexture2D* NewTex)
{
	UE_MVVM_SET_PROPERTY_VALUE(Portrait, NewTex);
}

UTexture2D* UMWVMCharacterStatus::GetPortrait() const
{
	return Portrait;
}
