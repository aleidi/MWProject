#include "Input/MWChargeInputProcessor.h"

#include "Engine/Engine.h"
#include "Gameplay/MWGameplayTags.h"

bool UMWChargeInputProcessor::IsChargeInputTag(const FGameplayTag& InInputTag) const
{
	return InInputTag.MatchesTagExact(MWGameplayTags::IATag_TPBattle_CharacterSkillSlot1_Charge)
		|| InInputTag.MatchesTagExact(MWGameplayTags::IATag_TPBattle_CharacterSkillSlot2_Charge)
		|| InInputTag.MatchesTagExact(MWGameplayTags::IATag_TPBattle_CharacterSkillSlot3_Charge);
}

void UMWChargeInputProcessor::HandlePressed(const FGameplayTag& InInputTag, const FMWChargeRuntimeConfig& InRuntimeConfig)
{
	if (!IsChargeInputTag(InInputTag))
	{
		return;
	}

	FMWChargeInputState& chargeState = ChargeStates.FindOrAdd(InInputTag);
	chargeState.bIsPressed = true;
	chargeState.RuntimeMaxChargeValue = InRuntimeConfig.MaxChargeValue;
	chargeState.RuntimeChargeRate = InRuntimeConfig.ChargeRate;
	chargeState.RuntimeDischargeRate = InRuntimeConfig.DischargeRate;
	chargeState.RuntimeChargeStartDelay = InRuntimeConfig.ChargeStartDelay;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			1.5f,
			FColor::Cyan,
			FString::Printf(
				TEXT("[Charge][Pressed] Tag=%s Hold=%.2f Charge=%.2f Full=%d Rate=%.2f Max=%.2f"),
				*InInputTag.ToString(),
				chargeState.HoldTime,
				chargeState.ChargeValue,
				chargeState.bIsFullyCharged ? 1 : 0,
				chargeState.RuntimeChargeRate,
				chargeState.RuntimeMaxChargeValue));
	}
}

bool UMWChargeInputProcessor::HandleReleased(const FGameplayTag& InInputTag, FGameplayTag& OutCastInputTag)
{
	OutCastInputTag = FGameplayTag();

	if (!IsChargeInputTag(InInputTag))
	{
		return false;
	}

	FMWChargeInputState* chargeState = ChargeStates.Find(InInputTag);
	if (!chargeState)
	{
		return false;
	}

	chargeState->bIsPressed = false;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			1.5f,
			FColor::Yellow,
			FString::Printf(
				TEXT("[Charge][Released] Tag=%s Hold=%.2f Charge=%.2f Full=%d"),
				*InInputTag.ToString(),
				chargeState->HoldTime,
				chargeState->ChargeValue,
				chargeState->bIsFullyCharged ? 1 : 0));
	}

	// フルチャージ: チャージタグを送出
	if (chargeState->bIsFullyCharged)
	{
		OutCastInputTag = InInputTag;
		ChargeStates.Remove(InInputTag);
		return true;
	}

	// タップ: チャージフロー内でReleaseを消費し、何もしない
	if (chargeState->HoldTime < chargeState->RuntimeChargeStartDelay)
	{
		ChargeStates.Remove(InInputTag);
		return true;
	}

	// フルチャージでもタップでもない場合: 減衰へ移行
	chargeState->HoldTime = chargeState->RuntimeChargeStartDelay;
	return true;
}

void UMWChargeInputProcessor::Tick(float InDeltaSeconds)
{
	TArray<FGameplayTag> tagsToRemove;

	for (auto& pair : ChargeStates)
	{
		FMWChargeInputState& chargeState = pair.Value;

		if (chargeState.bIsPressed)
		{
			chargeState.HoldTime += InDeltaSeconds;

			if (chargeState.HoldTime >= chargeState.RuntimeChargeStartDelay)
			{
				chargeState.ChargeValue = FMath::Min(
					chargeState.RuntimeMaxChargeValue,
					chargeState.ChargeValue + chargeState.RuntimeChargeRate * InDeltaSeconds);

				chargeState.bIsFullyCharged = chargeState.ChargeValue >= chargeState.RuntimeMaxChargeValue;
			}
		}
		else
		{
			chargeState.ChargeValue = FMath::Max(
				0.0f,
				chargeState.ChargeValue - chargeState.RuntimeDischargeRate * InDeltaSeconds);

			if (chargeState.ChargeValue <= 0.0f)
			{
				tagsToRemove.Add(pair.Key);
			}
		}
	}

	for (const FGameplayTag& tagToRemove : tagsToRemove)
	{
		ChargeStates.Remove(tagToRemove);
	}
}

void UMWChargeInputProcessor::Reset()
{
	ChargeStates.Reset();
}

FGameplayTag UMWChargeInputProcessor::ResolveNormalInputTagFromChargeTag(const FGameplayTag& InChargeInputTag) const
{
	if (InChargeInputTag.MatchesTagExact(MWGameplayTags::IATag_TPBattle_CharacterSkillSlot1_Charge))
	{
		return MWGameplayTags::IATag_TPBattle_CharacterSkillSlot1;
	}

	if (InChargeInputTag.MatchesTagExact(MWGameplayTags::IATag_TPBattle_CharacterSkillSlot2_Charge))
	{
		return MWGameplayTags::IATag_TPBattle_CharacterSkillSlot2;
	}

	if (InChargeInputTag.MatchesTagExact(MWGameplayTags::IATag_TPBattle_CharacterSkillSlot3_Charge))
	{
		return MWGameplayTags::IATag_TPBattle_CharacterSkillSlot3;
	}

	return FGameplayTag();
}
