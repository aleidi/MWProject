#include "GameplayAbility/Ability/MWChargeableSkill.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Data/GameplayAbility/ChargeableSkillData.h"
#include "Gameplay/MWGameplayTags.h"

void UMWChargeableSkill::OnChargeComplete(float ChargeTime)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, FString::Printf(TEXT("[%s] Charge Complete! Charge Time: %f"), *GetName(), ChargeTime));

	// Remove the charging effect
	if (ChargingEffectHandle.IsValid())
	{
		if(UMWAbilitySystemComponent* mwASC = GetMWAbilitySystemComponentFromActorInfo())
		{
			mwASC->RemoveActiveGameplayEffect(ChargingEffectHandle);
			ChargingEffectHandle.Invalidate();
		}
	}

	check(IsValid(Data));

	EMWInputChargeStage inputChargeStage = CalculateChargeStage(ChargeTime);

	// 寻找动画然后播放
	FMWInputChargeStageSkillAnim& skillAnim = Data->SkillAnims.FindOrAdd(inputChargeStage);
	if (skillAnim.Montage.IsNull())
	{
		UE_LOG(LogMWAbilitySystem, Warning, TEXT("Montage path is null"));

		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);

		return;
	}

	// 2. 尝试加载
	UAnimMontage* LoadedMontage = skillAnim.Montage.LoadSynchronous();

	// 3. 验证加载结果
	if (!IsValid(LoadedMontage))
	{
		UE_LOG(LogMWAbilitySystem, Error, TEXT("Failed to load montage: %s"), *skillAnim.Montage.ToString());

		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);

		return;
	}

	FString taskName = FString::Printf(TEXT("AT_%s[%s]"), *GetClass()->GetName(), *LoadedMontage->GetName());

	UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
	this, 
	FName(*taskName),
	LoadedMontage, 
	skillAnim.PlayRate,
	skillAnim.MontageSection);

	// 蓄力完成的特效？
}

void UMWChargeableSkill::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	// Use "ActivationBlockTags" to prevent multiple chargeable abilities from being activated simultaneously.
	// Double check here if "ActivationBlockTags" is not used.
 	UMWAbilitySystemComponent* mwASC = GetMWAbilitySystemComponentFromActorInfo();
	if (mwASC->HasMatchingGameplayTag(MWGameplayTags::Ability_Input_Charging))
	{
		UE_LOG(LogMWAbilitySystem, Warning, TEXT("Ability is already charging."));

		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);

		return;
	}

	if (!IsValid(Data))
	{
		UE_LOG(LogMWAbilitySystem, Warning, TEXT("Chargeable Skill Data is not valid."));

		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);

		return;
	}

	// TODO: 蓄力提示用的特效是否应该在这里处理或者通过其他动画播放呢？
	if (UMWGameplayData* GameplayData = MWSINGLETON->GetGameplayData())
	{
		TSubclassOf<UGameplayEffect> inputChargingEffectClass = GameplayData->GEInputCharging;

		if (inputChargingEffectClass)
		{
			// 创建 Gameplay Effect Context
			FGameplayEffectContextHandle EffectContext = mwASC->MakeEffectContext();
			EffectContext.AddSourceObject(this);

			// 创建 Gameplay Effect Spec
			FGameplayEffectSpecHandle SpecHandle = mwASC->MakeOutgoingSpec(inputChargingEffectClass, GetAbilityLevel(), EffectContext);

			if (SpecHandle.IsValid())
			{
				// 应用 Gameplay Effect 并保存 Handle
				ChargingEffectHandle = mwASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

				if (!ChargingEffectHandle.IsValid())
				{
					UE_LOG(LogMWAbilitySystem, Warning, TEXT("Failed to apply GEInputCharging"));
				}
			}
			else
			{
				UE_LOG(LogMWAbilitySystem, Error, TEXT("Failed to create GEInputCharging spec"));
			}
		}
		else
		{
			UE_LOG(LogMWAbilitySystem, Error, TEXT("GEInputCharging class is null or failed to load"));
		}
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);	
}

EMWInputChargeStage UMWChargeableSkill::CalculateChargeStage(float ChargeTime) const
{
	check(IsValid(Data));

	// TODO: 具体区间可能会受到以太波动值的影响，暂时先作为固定值处理

	if (ChargeTime < Data->Thresholds.TapThreshold)
	{
		return EMWInputChargeStage::NoCharge;
	}
	else if (ChargeTime >= Data->Thresholds.TapThreshold && ChargeTime < Data->Thresholds.PerfectMin)
	{
		return EMWInputChargeStage::Light;
	}
	else if (ChargeTime >= Data->Thresholds.PerfectMin && ChargeTime <= Data->Thresholds.PerfectMax)
	{
		return EMWInputChargeStage::Perfect;
	}
	else if (ChargeTime > Data->Thresholds.PerfectMax)
	{
		return EMWInputChargeStage::Overcharge;
	}

	return EMWInputChargeStage::NoCharge;
}
