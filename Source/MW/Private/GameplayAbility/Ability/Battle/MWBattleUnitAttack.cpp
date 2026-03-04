#include "GameplayAbility/Ability/Battle/MWBattleUnitAttack.h"

#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Animation/AnimSequenceBase.h"
#include "Animation/Notify/Battle/Combat/AN_ComboHit.h"
#include "Component/Battle/MWBattleUnitComponent.h"
#include "Component/Character/MWCharacterAnimControlComponent.h"
#include "Data/MWCharacterData.h"
#include "Gameplay/Battle/BattleUnit/MWBattleUnitAvatar.h"
#include "Gameplay/MWGameplayTags.h"
#include "MWLogChannels.h"

#include "GameplayAbility/Attribute/MWBattleAttributeSet.h"
#define MAX_COMBO_STEP 4

UMWBattleUnitAttack::UMWBattleUnitAttack(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ActivateTag = MWGameplayTags::Battle_Ability_UnitAttack;
	AvatarChangeTag = MWGameplayTags::Battle_AvatarChanged;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	ApproachStates.Reserve(MAX_COMBO_STEP);
}

void UMWBattleUnitAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	AActor* character = ActorInfo ? ActorInfo->AvatarActor.Get() : GetAvatarActorFromActorInfo(); 
	SetupSkillTable(character);

	// Setup skill table should be before reset approach states.
	// スキルテーブルの設定は、アプローチ状態のリセットの前に行う必要がある.
	BindDelegates();
	ResetApproachStates();
	ResetCombo();
}

void UMWBattleUnitAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	UnbindDelegates();
	ResetApproachStates();
	ResetCombo();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UMWBattleUnitAttack::OnCombo(const FGameplayEventData* Payload)
{	
	// Combo is finished, do nothing.
	// コンボが終了した場合、何もしない。
	if (CurrComboStep >= SkillTable.Num())
	{
		return;
	}

	// If currently doing approach, wait until approach is finished.
	// 接近が終了するまで待つ。
	if (IsDoingApproach())
	{
		return;
	}

	if (!Payload || !Payload->Target)
	{
		return;
	}

	AttackTargetActor = Payload->Target;

	AMWCharacter* character = Cast<AMWCharacter>(GetAvatarActorFromActorInfo());

	if (!character)
	{
		return;
	}

	// Calculate total damage for the combo.
	CalculateComboDamage();

	// TODO : Check action points before executing the combo.

	CurrComboType = EMWCharacterSkillComboType::Max;

	// Check the combo type and play the corresponding skill animation.
	// コンボタイプを確認して、対応するスキルアニメーションを再生する。
	if (Payload->EventTag == MWGameplayTags::Battle_ComboCentral)
	{
		CurrComboType = EMWCharacterSkillComboType::Central;
	}
	else if( Payload->EventTag == MWGameplayTags::Battle_ComboUpDown)
	{
		CurrComboType = EMWCharacterSkillComboType::UpDown;
	}
	else if (Payload->EventTag == MWGameplayTags::Battle_ComboLeftRight)
	{
		CurrComboType = EMWCharacterSkillComboType::LeftRight;
	}
	else
	{
		UE_LOG(LogMWBattle, Log, TEXT("UMWBattleUnitAttack::GameplayEventCallback: Tag[%s]"), *Payload->EventTag.ToString());
		return;
	}

	const FMWCharacterBattleSkillGroup* skillGroup = SkillTable.GetComboAt(CurrComboStep);

	if (!skillGroup)
	{
		UE_LOG(LogMWBattle, Warning, TEXT("UMWBattleUnitAttack::GameplayEventCallback: Skill group not found for combo step[%d]"), CurrComboStep);

		return;
	}

	const FMWCharacterBattleSkillData* skillData = skillGroup->GetSkill(CurrComboType);

	if (!skillData)
	{
		return;
	}

	// Set the original position flag if approach is required at the first combo step.
	// 最初のコンボステップで接近が必要な場合、OriginalPositionFlagを設定する。
	if (!IsOriginalPositionFlagSet())
	{
		OriginalPosition = character->GetActorLocation();

		SetOriginalPositionFlag(true);
	}

	// Check if the target is within skill range.
	// ターゲットがスキル範囲内にいるか確認する。
	bool bIsTargetInRange = false;
	{
		const float distance = FVector::Dist(AttackTargetActor->GetActorLocation(), character->GetActorLocation());

		bIsTargetInRange = distance <= skillData->CastRange;
	}

	UAnimMontage* approachAnim = skillData->bUseApproachAnimation ? SkillTable.ApproachAnimation : nullptr;

	if(skillData->bUseApproachAnimation && approachAnim == nullptr)
	{
		UE_LOG(LogMWBattle, Warning, TEXT("UMWBattleUnitAttack::GameplayEventCallback: Approach animation is null for combo type[%d] step[%d]"), (int)CurrComboType, CurrComboStep);
	}

	// If the target is out of range and requires approach, handle the approach logic first.
	// ターゲットが範囲外で接近が必要な場合、最初に接近ロジックを処理する。
	auto ShouldPlayApproachAnim = [approachAnim, bIsTargetInRange, skillData]() -> bool
	{
		return	approachAnim &&
				!bIsTargetInRange && 
				skillData->bRequiresApproach && 
				skillData->bUseApproachAnimation;
	};

	if (ShouldPlayApproachAnim())
	{
		if (PlayMontage(approachAnim, ApproachAnimBlendingOutDelegate, ApproachAnimEndedDelegate))
		{
			SetApproachState(CurrComboStep, true);

			// Get CharacterAnimControlComponent to start approach logic.
			// キャラクターアニメーションコントロールコンポーネントを取得して、接近ロジックを開始する。
			if (auto* animControlComp = character->FindComponentByClass<UMWCharacterAnimControlComponent>())
			{
				animControlComp->StartApproachTarget(AttackTargetActor.Get());
			}
		}
		// Early return to wait for approach animation to finish before playing the skill animation.
		// スキルアニメーションを再生する前に、接近アニメーションが終了するのを待つために早期リターン。
		return;
	}

	UAnimMontage* skillAnim = skillData->Animation;

	CalcualteComboHitNum(skillAnim);

	if (PlaySkillAnim(skillAnim))
	{
		// Maybe the skill has approach curve.
		// スキルにアプローチカーブがあるかもしれない。
		if (!bIsTargetInRange && skillData->bRequiresApproach && !skillData->bUseApproachAnimation)
		{
			// Get CharacterAnimControlComponent to start approach logic.
			// キャラクターアニメーションコントロールコンポーネントを取得して、接近ロジックを開始する。
			if (auto* animControlComp = character->FindComponentByClass<UMWCharacterAnimControlComponent>())
			{
				animControlComp->StartApproachTarget(AttackTargetActor.Get());
			}
		}
	}
}

void UMWBattleUnitAttack::OnComboHit(const FGameplayEventData* Payload)
{
	int32 hitDamage = 0;

	// If it's the last hit, assign the remaining damage to ensure total damage consistency.
	// 最後のヒットの場合、残りのダメージを割り当てて、総ダメージの一貫性を確保する。
	const UAN_ComboHit* hitObj = Cast<const UAN_ComboHit>(Payload->OptionalObject);

	float damageWeight = hitObj->DamageWeight;

	hitDamage = FMath::Floor(CurrComboTotalDmg * damageWeight);

	++CurrComboHitIdx;

	FString ename = StaticEnum<EMWAttackResult>()->GetNameStringByValue((int64)CurrComboResult);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("UMWBattleUnitAttack::OnComboHit: HitDamage[%d], HitRes[%s]"), hitDamage, *ename));

	// Apply damage to the target.
	// ターゲットにダメージを適用する.
	ApplyComboDamageToTarget(AttackTargetActor.Get(), hitObj->DamageWeight);
}

void UMWBattleUnitAttack::OnComboEnd()
{
	// Go back to the original position if approached.
	// 接近した場合、元の位置に戻る。
	if (AMWCharacter* character = Cast<AMWCharacter>(GetAvatarActorFromActorInfo()))
	{
		// If the owner has not moved, skip.
		// オーナーが移動していない場合、スキップする。
		if (FVector::DistXY(OriginalPosition, character->GetActorLocation()) > MaxDistanceFromInitialPosition)
		{
			if (UAnimMontage* returnApproachAnim = SkillTable.ReturnAnimation)
			{
				if (auto* animControlComp = character->FindComponentByClass<UMWCharacterAnimControlComponent>())
				{
					animControlComp->StartApproachPoint(OriginalPosition);
				}

				if (PlayMontage(returnApproachAnim, ReturnAnimBlendingOutDelegate, ReturnAnimEndedDelegate))
				{
					SetApproachState(CurrComboStep, true);

					return;
				}
			}
		}
	}

	SetOriginalPositionFlag(false);
	ResetApproachStates();
	ResetCombo();
}

void UMWBattleUnitAttack::OnAvatarChange(const FGameplayEventData* Payload)
{
	SetupSkillTable(GetAvatarActorFromActorInfo());
}

void UMWBattleUnitAttack::OnApproachAnimBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, FString::Printf(TEXT("UMWBattleUnitAttack::OnApproachAnimBlendingOut: Montage[%s] Interrupted[%s]"), *GetNameSafe(Montage), bInterrupted ? TEXT("True") : TEXT("False")));

	if (AMWCharacter* character = Cast<AMWCharacter>(GetAvatarActorFromActorInfo()))
	{
		if (auto* animControlComp = character->FindComponentByClass<UMWCharacterAnimControlComponent>())
		{
			animControlComp->EndApproach(true);
		}
	}
}

void UMWBattleUnitAttack::OnApproachAnimEnded(UAnimMontage* Montage, bool bInterrupted)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, FString::Printf(TEXT("UMWBattleUnitAttack::OnApproachAnimEnded: Montage[%s] Interrupted[%s]"), *GetNameSafe(Montage), bInterrupted ? TEXT("True") : TEXT("False")));

	const FMWCharacterBattleSkillGroup* skillGroup = SkillTable.GetComboAt(CurrComboStep);

	if (!skillGroup)
	{
		UE_LOG(LogMWBattle, Warning, TEXT("UMWBattleUnitAttack::GameplayEventCallback: Skill group not found for combo step[%d]"), CurrComboStep);

		return;
	}

	const FMWCharacterBattleSkillData* skillData = skillGroup->GetSkill(CurrComboType);

	if (!skillData)
	{
		return;
	}

	UAnimMontage* skillAnim = skillData->Animation;

	CalcualteComboHitNum(skillAnim);

	PlaySkillAnim(skillAnim);

	SetApproachState(CurrComboStep, false);
}

void UMWBattleUnitAttack::OnReturnAnimBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, FString::Printf(TEXT("UMWBattleUnitAttack::OnReturnAnimBlendingOut: Montage[%s] Interrupted[%s]"), *GetNameSafe(Montage), bInterrupted ? TEXT("True") : TEXT("False")));

	if (AMWCharacter* character = Cast<AMWCharacter>(GetAvatarActorFromActorInfo()))
	{
		if (auto* animControlComp = character->FindComponentByClass<UMWCharacterAnimControlComponent>())
		{
			animControlComp->EndApproach(true);
		}
	}
}

void UMWBattleUnitAttack::OnReturnAnimEnded(UAnimMontage* Montage, bool bInterrupted)
{
	SetOriginalPositionFlag(false);
	ResetApproachStates();
	ResetCombo();
}

void UMWBattleUnitAttack::OnSkillAnimBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, FString::Printf(TEXT("UMWBattleUnitAttack::OnSkillAnimBlendingOut: Montage[%s] Interrupted[%s]"), *GetNameSafe(Montage), bInterrupted ? TEXT("True") : TEXT("False")));

	// TODO : Process some timing effects here if needed.
}

void UMWBattleUnitAttack::OnSkillAnimEnded(UAnimMontage* Montage, bool bInterrupted)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, FString::Printf(TEXT("UMWBattleUnitAttack::OnSkillAnimEnded: Montage[%s] Interrupted[%s]"), *GetNameSafe(Montage), bInterrupted ? TEXT("True") : TEXT("False")));
	
	// If all combo steps are done, end the combo.
	// すべてのコンボステップが完了した場合、コンボを終了する。
	if (!bInterrupted && CurrComboStep >= SkillTable.Num())
	{
		OnComboEnd();
	}

	// TODO : Check character's action point or any other conditions to decide whether to end combo.
}

void UMWBattleUnitAttack::SetupSkillTable(AActor* AvatarActor)
{
	if (AvatarActor)
	{
		if (UMWBattleUnitComponent* battleUnitComp = AvatarActor->FindComponentByClass<UMWBattleUnitComponent>())
		{
			if (const UMWCharacterBattleSkillDataAsset* table = battleUnitComp->GetSkillData())
			{
				SkillTable.ApproachAnimation = table->ApproachAnimation;
				SkillTable.ReturnAnimation = table->ReturnAnimation;
				const int32 maxComboNum  = table->SkillCombos.Num();
				for (int32 i = 0; i < maxComboNum; ++i)
				{
					SkillTable.SetComboAt(i, table->SkillCombos[i]);
				}
			}
			else
			{
				UE_LOG(LogMWBattle, Warning, TEXT("UMWBattleUnitAttack::SetupSkillTable: SkillTable is null in character=%s"), *AvatarActor->GetName());
			}
		}
	}
}

void UMWBattleUnitAttack::ResetCombo()
{
	AttackTargetActor = nullptr;

	CurrComboStep = 0;

	CurrComboTotalDmg = 0;

	CurrComboResult = EMWAttackResult::Max;

	ComboHitNum = 0;

	CurrComboHitIdx = 0;
}

void UMWBattleUnitAttack::BindDelegates()
{
	ComboEventHandle = GetAbilitySystemComponentFromActorInfo_Ensured()->GenericGameplayEventCallbacks.FindOrAdd(ComboTag).AddUObject(this, &ThisClass::OnCombo);
	ComboHitEventHandle = GetAbilitySystemComponentFromActorInfo_Ensured()->GenericGameplayEventCallbacks.FindOrAdd(ComboHitTag).AddUObject(this, &ThisClass::OnComboHit);
	AvatarChangeEventHandle = GetAbilitySystemComponentFromActorInfo_Ensured()->GenericGameplayEventCallbacks.FindOrAdd(AvatarChangeTag).AddUObject(this, &ThisClass::OnAvatarChange);

	SkillAnimBlendingOutDelegate.BindUObject(this, &ThisClass::OnSkillAnimBlendingOut);
	SkillAnimEndedDelegate.BindUObject(this, &ThisClass::OnSkillAnimEnded);
	ApproachAnimEndedDelegate.BindUObject(this, &ThisClass::OnApproachAnimEnded);
	ApproachAnimBlendingOutDelegate.BindUObject(this, &ThisClass::OnApproachAnimBlendingOut);
	ReturnAnimBlendingOutDelegate.BindUObject(this, &ThisClass::OnReturnAnimBlendingOut);
	ReturnAnimEndedDelegate.BindUObject(this, &ThisClass::OnReturnAnimEnded);
}

void UMWBattleUnitAttack::UnbindDelegates()
{
	if (auto* asc = GetAbilitySystemComponentFromActorInfo())
	{
		if (AvatarChangeEventHandle.IsValid())
		{
			asc->GenericGameplayEventCallbacks.FindOrAdd(AvatarChangeTag).Remove(AvatarChangeEventHandle);
		}

		if (ComboEventHandle.IsValid())
		{
			asc->GenericGameplayEventCallbacks.FindOrAdd(ComboTag).Remove(ComboEventHandle);
		}

		if (ComboHitEventHandle.IsValid())
		{
			asc->GenericGameplayEventCallbacks.FindOrAdd(ComboHitTag).Remove(ComboHitEventHandle);
		}
	}
}

bool UMWBattleUnitAttack::PlayMontage(UAnimMontage* InMontage, FOnMontageBlendingOutStarted& BlendingOutDelegate, FOnMontageEnded& EndDelegate)
{
	if (AMWCharacter* character = Cast<AMWCharacter>(GetAvatarActorFromActorInfo()))
	{
		if (USkeletalMeshComponent* skMeshComp = character->GetComponentByClass<USkeletalMeshComponent>())
		{
			if (UAnimInstance* animInst = Cast<UAnimInstance>(skMeshComp->GetAnimInstance()))
			{
				bool bPlayedSuccessfully = animInst->Montage_Play(InMontage) > 0.f;

				if (bPlayedSuccessfully)
				{
					animInst->Montage_SetBlendingOutDelegate(BlendingOutDelegate, InMontage);

					animInst->Montage_SetEndDelegate(EndDelegate, InMontage);

					return true;
				}
			}
		}
	}

	return false;
}

bool UMWBattleUnitAttack::IsDoingApproach() const
{
	return ApproachStates[CurrComboStep];
}

void UMWBattleUnitAttack::SetApproachState(int32 CurrentComboStep, bool NewState)
{
	bool& state = ApproachStates.FindOrAdd(CurrentComboStep);

	state = NewState;
}

void UMWBattleUnitAttack::ResetApproachStates()
{
	const int32 maxCombo = SkillTable.Num();
	for (int32 i = 0; i < maxCombo; ++i)
	{
		auto& state = ApproachStates.FindOrAdd(i);

		state = false;
	}
}

bool UMWBattleUnitAttack::IsOriginalPositionFlagSet() const
{
	return bIsOriginalPositionSet;
}

void UMWBattleUnitAttack::SetOriginalPositionFlag(bool NewSetate)
{
	bIsOriginalPositionSet = NewSetate;
}

bool UMWBattleUnitAttack::PlaySkillAnim(UAnimMontage* InAnimation)
{
	if (InAnimation)
	{
		if (PlayMontage(InAnimation, SkillAnimBlendingOutDelegate, SkillAnimEndedDelegate))
		{
			++CurrComboStep;

			return true;
		}
	}
	else
	{
		UE_LOG(LogMWBattle, Warning, TEXT("UMWBattleUnitAttack::GameplayEventCallback: Skill animation not found for combo type[%d] step[%d]"), (int)CurrComboType, CurrComboStep);
	}

	return false;
}

void UMWBattleUnitAttack::CalculateComboDamage()
{
	CurrComboTotalDmg = 9999;

	CurrComboResult = EMWAttackResult::Critical;

	ApplyComboTotalDamageToSource();
}

void UMWBattleUnitAttack::CalcualteComboHitNum(UAnimSequenceBase* InAnim)
{
	if (InAnim)
	{
		for (auto& notify : InAnim->Notifies)
		{
			if (notify.Notify && notify.Notify->IsA<UAN_ComboHit>())
			{
				++ComboHitNum;
			}
		}
	}
}

void UMWBattleUnitAttack::ApplyComboTotalDamageToSource()
{
	UAbilitySystemComponent* sourceASC = GetAbilitySystemComponentFromActorInfo();

	if (sourceASC && GE_ComboTotalDamage)
	{
		FGameplayEffectContextHandle contextHandle = sourceASC->MakeEffectContext();
		contextHandle.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());
		
		FGameplayEffectSpecHandle specHandle = sourceASC->MakeOutgoingSpec(GE_ComboTotalDamage, 1.f, contextHandle);
		
		if (specHandle.IsValid())
		{	
			sourceASC->ApplyGameplayEffectSpecToSelf(*specHandle.Data.Get());
		}
	}
}

void UMWBattleUnitAttack::ApplyComboDamageToTarget(const AActor* InTargetActor, float ComboHitRatio)
{
	if (!InTargetActor)
	{
		return;
	}

	UAbilitySystemComponent* sourceASC = GetAbilitySystemComponentFromActorInfo();
	UAbilitySystemComponent* targetASC = InTargetActor->GetComponentByClass<UAbilitySystemComponent>();

	if (sourceASC && targetASC && GE_ComboHit)
	{
		FGameplayEffectContextHandle contextHandle = sourceASC->MakeEffectContext();
		contextHandle.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());
		
		FGameplayEffectSpecHandle specHandle = sourceASC->MakeOutgoingSpec(GE_ComboHit, 1.f, contextHandle);
		
		if (specHandle.IsValid())
		{
			specHandle.Data->SetSetByCallerMagnitude(MWGameplayTags::GP_SetByCaller_ComboHitRatio, ComboHitRatio);
			
			sourceASC->ApplyGameplayEffectSpecToTarget(*specHandle.Data.Get(), targetASC);

			auto* attrSet = targetASC->GetSet<UMWBattleAttributeSet>();
			if (attrSet)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("UMWBattleUnitAttack::ApplyComboDamageToTarget: Target HP[%f]"), attrSet->GetHealth()));
			}
		}
	}
}
