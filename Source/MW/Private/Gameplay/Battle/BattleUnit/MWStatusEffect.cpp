#include "Gameplay/Battle/BattleUnit/MWStatusEffect.h"

bool MWBattle::FMWStatusEffect::OnTrigger(UMWBattleUnit* TargetUnit, ETriggerTiming Timing, void* Context)
{
	if (Timing == DurationUpdateTiming && Duration > 0)
	{
		--Duration;

		if (0 == Duration)
		{
			return false; // Indicate that the effect should be removed	// 効果を削除することを示す
		}
	}

	if (Timing == TriggerTiming)
	{
		OnApply(TargetUnit, Context);
	}

	return true;
}

void MWBattle::FMWStatusEffect::ForceExpire()
{
	Duration = 0;
}

MWBattle::FMWStatusEffectManager::~FMWStatusEffectManager()
{
	ActiveEffects.Empty();

	OwnerUnit = nullptr;
}

void MWBattle::FMWStatusEffectManager::AddEffect(TSharedPtr<FMWStatusEffect> NewEffect)
{
	if(!IsValid())
	{
		return;
	}

	if (!NewEffect.IsValid())
	{
		return;
	}

	if (!NewEffect->IsStackable())
	{
		for (const auto& Effect : ActiveEffects)
		{
			if (Effect == NewEffect)
			{
				// Effect already exists, do not add again
				// 効果はすでに存在しているため、再度追加しない
				return;
			}
		}
	}

	ActiveEffects.Emplace(NewEffect);

	// Trigger the effect when applied
	// 適用時に効果を発動する
	NewEffect->OnTrigger(OwnerUnit, ETriggerTiming::OnApply);

	if(OnEffectAdded.IsBound())
	{
		OnEffectAdded.Broadcast(NewEffect->GetName());
	}
}

void MWBattle::FMWStatusEffectManager::RemoveEffect(TSharedPtr<FMWStatusEffect> EffectToRemove)
{
	if (!IsValid())
	{
		return;
	}

	if (!EffectToRemove.IsValid())
	{
		return;
	}

	// Trigger the effect when removed
	// 削除時に効果を発動する
	EffectToRemove->OnTrigger(OwnerUnit, ETriggerTiming::OnRemove);

	ActiveEffects.Remove(EffectToRemove);

	if(OnEffectRemoved.IsBound())
	{
		OnEffectRemoved.Broadcast(EffectToRemove->GetName());
	}
}

void MWBattle::FMWStatusEffectManager::TriggerEffects(ETriggerTiming Timing, void* Context)
{
	if (!IsValid())
	{
		return;
	}

	for (int32 i = ActiveEffects.Num() - 1; i >= 0; --i)
	{
		const auto& effect = ActiveEffects[i];

		if (effect.IsValid())
		{
			bool bShouldRemain = effect->OnTrigger(OwnerUnit, Timing, Context);

			if (!bShouldRemain)
			{
				// Effect has expired, remove it
				// 効果が期限切れになったため、削除する
				ActiveEffects.RemoveAt(i);
			}
		}
	}
}
