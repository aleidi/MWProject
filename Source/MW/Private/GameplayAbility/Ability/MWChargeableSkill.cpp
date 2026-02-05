#include "GameplayAbility/Ability/MWChargeableSkill.h"

#include "AbilitySystemGlobals.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Data/GameplayAbility/ChargeableSkillData.h"
#include "GameplayTagContainer.h"
#include "Gameplay/MWGameplayTags.h"
#include "Gameplay/MWGameplayUtility.h"

void UMWChargeableSkill::OnChargeComplete(float ChargeTime)
{
	UMWAbilitySystemComponent* mwASC = GetMWAbilitySystemComponentFromActorInfo();
	if (!mwASC)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);

		return;
	}

	// Remove the charging effect
	if (ChargingEffectHandle.IsValid())
	{
		mwASC->RemoveActiveGameplayEffect(ChargingEffectHandle);
		ChargingEffectHandle.Invalidate();
	}

	// Check if the avatar has interruptible tag
	if (mwASC->HasMatchingGameplayTag(MWGameplayTags::Ability_ActionUninterruptible))
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("[%s] Charge interrupted due to uninterruptible state."), *GetName()));

		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);

		return;
	}	

	mwASC->AddLooseGameplayTag(MWGameplayTags::Ability_ActionUninterruptible);

	check(IsValid(Data));

	CurrentChargeStage = CalculateChargeStage(ChargeTime);

	// Add input charge stage tag to ability system component
	AddedChargeStageTag = UWMGameplayUtility::GetInputChargeStageTag(CurrentChargeStage);
	if (AddedChargeStageTag.IsValid())
	{
		mwASC->AddLooseGameplayTag(AddedChargeStageTag);
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, FString::Printf(TEXT("[%s] Applied charge stage tag: %s"), *GetName(), *AddedChargeStageTag.ToString()));
	}

	// ==================== 攻击判定 ====================
	// 在动画播放前就完成目标捕获（Hit Detection）
	CaptureTargets();

	// 寻找动画然后播放
	FMWInputChargeStageSkillAnim& skillAnim = Data->SkillAnims.FindOrAdd(CurrentChargeStage);
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

	UAbilityTask_PlayMontageAndWait* animTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
	this, 
	FName(*taskName),
	LoadedMontage, 
	skillAnim.PlayRate,
	skillAnim.MontageSection);

	animTask->OnCompleted.AddDynamic(this, &UMWChargeableSkill::OnMontageCompleted);
	animTask->OnBlendedIn.AddDynamic(this, &UMWChargeableSkill::OnMontageBlendIn);
	animTask->OnBlendOut.AddDynamic(this, &UMWChargeableSkill::OnMontageBlendOut);
	animTask->OnInterrupted.AddDynamic(this, &UMWChargeableSkill::OnMontageInterrupted);
	animTask->OnCancelled.AddDynamic(this, &UMWChargeableSkill::OnMontageCancelled);

	animTask->ReadyForActivation();

	// 蓄力完成的特效？

	// 攻击判定

	// 伤害应用
}

void UMWChargeableSkill::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	// Use "ActivationBlockTags" to prevent multiple chargeable abilities from being activated simultaneously.
	// Double check here if "ActivationBlockTags" is not used.
	//「ActivationBlockTags」を使用して、複数のチャージ可能なアビリティが同時にアクティブ化されるのを防ぐ。
	// ここで「ActivationBlockTags」が使用されていない場合は、再度確認する。
 	UMWAbilitySystemComponent* mwASC = GetMWAbilitySystemComponentFromActorInfo();
	if (mwASC->HasMatchingGameplayTag(MWGameplayTags::Ability_InputCharge_Charging))
	{
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
		TSubclassOf<UGameplayEffect> inputChargingEffectClass = GameplayData->GE_InputCharging;

		if (inputChargingEffectClass)
		{
			FGameplayEffectContextHandle EffectContext = mwASC->MakeEffectContext();
			EffectContext.AddSourceObject(this);

			FGameplayEffectSpecHandle SpecHandle = mwASC->MakeOutgoingSpec(inputChargingEffectClass, GetAbilityLevel(), EffectContext);

			if (SpecHandle.IsValid())
			{
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

void UMWChargeableSkill::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	// 清理捕获的目标数据
	ClearCapturedTargets();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
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

void UMWChargeableSkill::OnMontageCompleted()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, FString::Printf(TEXT("[%s] Montage Completed!"), *GetName()));

	ClearTagForThisAblity();

	if (IsActive())
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}

void UMWChargeableSkill::OnMontageBlendIn()
{
}

void UMWChargeableSkill::OnMontageBlendOut()
{
}

void UMWChargeableSkill::OnMontageInterrupted()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, FString::Printf(TEXT("[%s] Montage Interrupted!"), *GetName()));

	if (IsActive())
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}

void UMWChargeableSkill::OnMontageCancelled()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, FString::Printf(TEXT("[%s] Montage Cancelled!"), *GetName()));

	if (IsActive())
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}

void UMWChargeableSkill::ClearTagForThisAblity()
{
	UMWAbilitySystemComponent* mwASC = GetMWAbilitySystemComponentFromActorInfo();
	if (!mwASC)
	{
		return;
	}

	if (AddedChargeStageTag.IsValid() && mwASC->HasMatchingGameplayTag(AddedChargeStageTag))
	{
		mwASC->RemoveLooseGameplayTag(AddedChargeStageTag);
	}

	// Clear Uninterruptible tag if exists
	if (mwASC->HasMatchingGameplayTag(MWGameplayTags::Ability_ActionUninterruptible))
	{
		mwASC->RemoveLooseGameplayTag(MWGameplayTags::Ability_ActionUninterruptible);
	}	
}

void UMWChargeableSkill::CaptureTargets()
{
	AActor* avatarActor = GetAvatarActorFromActorInfo();
	if(!avatarActor)
	{
		UE_LOG(LogMWAbilitySystem, Warning, TEXT("[%s] Avatar actor is null, cannot capture targets"), *GetName());
		return;
	}

	check(IsValid(Data));

	// Get collision check parameters from skill data
	FMWInputChargeStageSkillAnim& skillAnim = Data->SkillAnims.FindOrAdd(CurrentChargeStage);

	// Use sphere collision
	TArray<AActor*> hitActors;
	TArray<AActor*> ignoreActors;
	ignoreActors.Add(avatarActor);

	// Perform sphere overlap check in front of character
	FVector startLocation = avatarActor->GetActorLocation();
	FVector forwardVector = avatarActor->GetActorForwardVector();

	// 技能数据应该包含检测参数，这里暂时硬编码
	float detectionRadius = 300.f;
	float detectionDistance = 500.f;
	TArray<TEnumAsByte<EObjectTypeQuery>> detectObjectTypes = {
		UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn)
	};

	FVector detectionCenter = startLocation + forwardVector * detectionDistance * 0.5f;

	bool bHit = UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(),
		detectionCenter,
		detectionRadius,
		detectObjectTypes,
		APawn::StaticClass(),
		ignoreActors,
		hitActors
	);

	if (bHit && hitActors.Num() > 0)
	{
		// Create TargetData to store hit actors
		FGameplayAbilityTargetData_ActorArray* targetData = new FGameplayAbilityTargetData_ActorArray();

		// Filter valid targets
		for(AActor* hitActor : hitActors)
		{
			// 可以添加额外的过滤条件：
			// 是否是敌对单位
			// 是否拥有asc
			// 是否在前方扇形范围
			UAbilitySystemComponent* targetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(hitActor);
			if (targetASC)
			{
				targetData->TargetActorArray.Add(hitActor);
			}
		}

		if (targetData->TargetActorArray.Num() > 0)
		{
			CapturedTargetData.Add(targetData);

			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan,
				FString::Printf(TEXT("[%s] Captured %d targets"),
					*GetName(), targetData->TargetActorArray.Num()));
		}
		else
		{
			delete targetData;
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow,
			FString::Printf(TEXT("[%s] No targets in range"), *GetName()));
	}


	DrawDebugSphere(GetWorld(), detectionCenter, detectionRadius, 16,
		bHit ? FColor::Green : FColor::Red, false, 2.f);
}

void UMWChargeableSkill::OnDamageNotify(FGameplayTag DamageEventTag, float DamageMultiplier, int32 HitIndex)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow,
		FString::Printf(TEXT("[%s] Damage Notify: %s, Multiplier: %.2f, Hit: %d"),
			*GetName(), *DamageEventTag.ToString(), DamageMultiplier, HitIndex));

	ApplyDamageToTargets(DamageEventTag, DamageMultiplier, HitIndex);
}

void UMWChargeableSkill::ApplyDamageToTargets(FGameplayTag DamageEventTag, float DamageMultiplier, int32 HitIndex)
{
	if(!CapturedTargetData.IsValid(0))
	{
		UE_LOG(LogMWAbilitySystem, Warning, TEXT("[%s] No captured targets for damage application"), *GetName());
		return;
	}

	UMWAbilitySystemComponent* sourceASC = GetMWAbilitySystemComponentFromActorInfo();
	if (!sourceASC)
	{
		return;
	}

	// Create damage effect spec
	FGameplayEffectSpecHandle damageSpecHandle = MakeDamageEffectSpec(DamageMultiplier);
	if(!damageSpecHandle.IsValid())
	{
		UE_LOG(LogMWAbilitySystem, Error, TEXT("[%s] Failed to create damage effect spec"), *GetName());
		return;
	}

	// Get target data
	const FGameplayAbilityTargetData* targetDataPtr = CapturedTargetData.Get(0);
	if (!targetDataPtr)
	{
		return;
	}

	// convert to ActorArray type
	const FGameplayAbilityTargetData_ActorArray* actorArrayData = static_cast<const FGameplayAbilityTargetData_ActorArray*>(targetDataPtr);
	if (!actorArrayData)
	{
		return;
	}

	// Apply damage to each target actor
	int32 damageAppliedCount = 0;
	for (TWeakObjectPtr<AActor> targetActorPtr : actorArrayData->TargetActorArray)
	{
		AActor* targetActor = targetActorPtr.Get();
		if (!IsValid(targetActor))
		{
			continue;
		}

		UAbilitySystemComponent* targetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(targetActor);
		if (!targetASC)
		{
			continue;
		}

		// Apply damage effect spec
		FActiveGameplayEffectHandle appliedHandle = sourceASC->ApplyGameplayEffectSpecToTarget(
			*damageSpecHandle.Data.Get(),
			targetASC
		);

		if (appliedHandle.WasSuccessfullyApplied())
		{
			++damageAppliedCount;

			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Orange,
				FString::Printf(TEXT("[%s] Applied damage to: %s"),
					*GetName(), *targetActor->GetName()));
		}
	}

	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green,
		FString::Printf(TEXT("[%s] Total damage applied to %d targets"),
			*GetName(), damageAppliedCount));
}

FGameplayEffectSpecHandle UMWChargeableSkill::MakeDamageEffectSpec(float DamageMultiplier)
{
	UMWAbilitySystemComponent* sourceASC = GetMWAbilitySystemComponentFromActorInfo();
	if (!sourceASC)
	{
		return FGameplayEffectSpecHandle();
	}

	// Get GameplayEffect class from skill data or GameplayData
	TSubclassOf<UGameplayEffect> damageEffectClass = nullptr;

	if(UMWGameplayData* gameplayData = MWSINGLETON->GetGameplayData())
	{
		damageEffectClass = gameplayData->GE_Damage;
	}

	if (!damageEffectClass)
	{
		UE_LOG(LogMWAbilitySystem, Error, TEXT("[%s] Damage GameplayEffect class is null"), *GetName());
		return FGameplayEffectSpecHandle();
	}

	// Create effect context
	FGameplayEffectContextHandle effectContext = sourceASC->MakeEffectContext();
	effectContext.AddSourceObject(this);
	effectContext.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());

	// Create effect spec
	FGameplayEffectSpecHandle specHandle = sourceASC->MakeOutgoingSpec(
		damageEffectClass,
		GetAbilityLevel(),
		effectContext
	);

	if (specHandle.IsValid())
	{
		// Set damage multiplier based on charge stage
		float chargeStageDamageMultiplier = 1.f;
		switch (CurrentChargeStage)
		{
			case EMWInputChargeStage::NoCharge:
				chargeStageDamageMultiplier = 0.5f;
				break;
			case EMWInputChargeStage::Light:
				chargeStageDamageMultiplier = 1.0f;
				break;
			case EMWInputChargeStage::Perfect:
				chargeStageDamageMultiplier = 1.5f;
				break;
			case EMWInputChargeStage::Overcharge:
				chargeStageDamageMultiplier = 2.0f;
				break;
		}

		// Apply to total multiplier
		float FinalMultiplier = chargeStageDamageMultiplier * DamageMultiplier;

		// 通过 SetByCaller 或 Modifier 设置伤害值
		// 方式1: 使用 SetByCaller (需要在 GE 中配置对应的 DataTag)
		// SpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("Data.Damage"), FinalMultiplier);

		// 方式2: 直接修改 Modifier
		// SpecHandle.Data->Modifiers[0].ModifierMagnitude.SetValue(FinalMultiplier);
	}

	return specHandle;
}

void UMWChargeableSkill::ClearCapturedTargets()
{
	CapturedTargetData.Clear();
}
