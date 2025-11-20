#include "GameplayAbility/Ability/Battle/MWBattleUnitAttack.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Component/Character/MWBattleUnitComponent.h"
#include "Component/Character/MWCharacterAnimControlComponent.h"
#include "Gameplay/Battle/BattleUnit/MWBattleUnitAvatar.h"
#include "Gameplay/MWGameplayTags.h"
#include "MWLogChannels.h"

#define MAX_COMBO_STEP 4

UMWBattleUnitAttack::UMWBattleUnitAttack(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ActivateTag = MWGameplayTags::GP_Battle_Ability_UnitAttack;
	AvatarChangeTag = MWGameplayTags::GP_Battle_AvatarChanged;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	ApproachStates.Reserve(MAX_COMBO_STEP);
}

void UMWBattleUnitAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	BindDelegates();

	AActor* avatar = ActorInfo ? ActorInfo->AvatarActor.Get() : GetAvatarActorFromActorInfo(); 
	SetupSkillTable(avatar);

	ResetApproachStates();

	ResetCombo();
}

void UMWBattleUnitAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	UnbindDelegates();

	ResetCombo();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
UE_DISABLE_OPTIMIZATION
void UMWBattleUnitAttack::OnCombo(const FGameplayEventData* Payload)
{	
	// Combo is finished, do nothing.
	// コンボが終了した場合、何もしない。
	if (ComboStep >= SkillTable.Num())
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

	AMWBattleUnitAvatar* avatar = Cast<AMWBattleUnitAvatar>(GetAvatarActorFromActorInfo());

	if (!avatar)
	{
		return;
	}

	// TODO : Check action points before executing the combo.

	ComboType = EMWCharacterSkillComboType::Max;

	// Check the combo type and play the corresponding skill animation.
	// コンボタイプを確認して、対応するスキルアニメーションを再生する。
	if (Payload->EventTag == MWGameplayTags::GP_Battle_ComboCentral)
	{
		ComboType = EMWCharacterSkillComboType::Central;
	}
	else if( Payload->EventTag == MWGameplayTags::GP_Battle_ComboUpDown)
	{
		ComboType = EMWCharacterSkillComboType::UpDown;
	}
	else if (Payload->EventTag == MWGameplayTags::GP_Battle_ComboLeftRight)
	{
		ComboType = EMWCharacterSkillComboType::LeftRight;
	}
	else
	{
		UE_LOG(LogMWBattle, Log, TEXT("UMWBattleUnitAttack::GameplayEventCallback: Tag[%s]"), *Payload->EventTag.ToString());
		return;
	}

	const FMWCharacterBattleSkillGroup* skillGroup = SkillTable.GetComboAt(ComboStep);

	if (!skillGroup)
	{
		UE_LOG(LogMWBattle, Warning, TEXT("UMWBattleUnitAttack::GameplayEventCallback: Skill group not found for combo step[%d]"), ComboStep);

		return;
	}

	const FMWCharacterBattleSkillData* skillData = skillGroup->GetSkill(ComboType);

	if (!skillData)
	{
		return;
	}

	// Set the original position flag if approach is required at the first combo step.
	// 最初のコンボステップで接近が必要な場合、OriginalPositionFlagを設定する。
	if (!IsOriginalPositionFlagSet())
	{
		OriginalPosition = avatar->GetActorLocation();

		SetOriginalPositionFlag(true);
	}

	// Check if the target is within skill range.
	// ターゲットがスキル範囲内にいるか確認する。
	bool bIsTargetInRange = false;
	{
		const float distance = FVector::Dist(Payload->Target->GetActorLocation(), avatar->GetActorLocation());

		bIsTargetInRange = distance <= skillData->CastRange;
	}

	UAnimMontage* approachAnim = skillData->bUseApproachAnimation ? SkillTable.ApproachAnimation : nullptr;

	if(skillData->bUseApproachAnimation && approachAnim == nullptr)
	{
		UE_LOG(LogMWBattle, Warning, TEXT("UMWBattleUnitAttack::GameplayEventCallback: Approach animation is null for combo type[%d] step[%d]"), (int)ComboType, ComboStep);
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
		// If currently doing skill approach, end it before playing the approach animation.
		// スキル接近を現在行っている場合、接近アニメーションを再生する前に終了する。
		if (auto* animControlComp = avatar->FindComponentByClass<UMWCharacterAnimControlComponent>())
		{
			if (animControlComp->IsApproaching())
			{
				animControlComp->EndApproach(false);
			}
		}

		if (PlayMontage(approachAnim, ApproachAnimBlendingOutDelegate, ApproachAnimEndedDelegate))
		{
			SetApproachState(ComboStep, true);

			// Get CharacterAnimControlComponent to start approach logic.
			// キャラクターアニメーションコントロールコンポーネントを取得して、接近ロジックを開始する。
			if (auto* animControlComp = avatar->FindComponentByClass<UMWCharacterAnimControlComponent>())
			{
				animControlComp->StartApproachTarget(Payload->Target.Get());
			}
		}
		// Early return to wait for approach animation to finish before playing the skill animation.
		// スキルアニメーションを再生する前に、接近アニメーションが終了するのを待つために早期リターン。
		return;
	}

	UAnimMontage* skillAnim = skillData->Animation;

	if(skillAnim)
	{
		// If currently doing approach, end it before playing the skill animation.
		// スキルアニメーションを再生する前に、現在接近している場合は終了する。
		if (auto* animControlComp = avatar->FindComponentByClass<UMWCharacterAnimControlComponent>())
		{
			if (animControlComp->IsApproaching())
			{
				animControlComp->EndApproach(false);
			}
		}

		if (PlaySkillAnim(skillAnim))
		{
			// Maybe the skill has approach curve.
			// スキルにアプローチカーブがあるかもしれない。
			if (!bIsTargetInRange && skillData->bRequiresApproach && !skillData->bUseApproachAnimation)
			{
				// Get CharacterAnimControlComponent to start approach logic.
				// キャラクターアニメーションコントロールコンポーネントを取得して、接近ロジックを開始する。
				if (auto* animControlComp = avatar->FindComponentByClass<UMWCharacterAnimControlComponent>())
				{
					animControlComp->StartApproachTarget(Payload->Target.Get());
				}
			}
		}
	}
	else
	{
		UE_LOG(LogMWBattle, Warning, TEXT("UMWBattleUnitAttack::GameplayEventCallback: Skill animation not found for combo type[%d] step[%d]"), (int)ComboType, ComboStep);
	}
}

void UMWBattleUnitAttack::OnComboEnd()
{
	// Go back to the original position if approached.
	// 接近した場合、元の位置に戻る。
	if (AMWBattleUnitAvatar* avatar = Cast<AMWBattleUnitAvatar>(GetAvatarActorFromActorInfo()))
	{
		// If the owner has not moved, skip.
		// オーナーが移動していない場合、スキップする。
		if (FVector::DistXY(OriginalPosition, avatar->GetActorLocation()) > MaxDistanceFromInitialPosition)
		{
			if (UAnimMontage* returnApproachAnim = SkillTable.ReturnAnimation)
			{
				if (auto* animControlComp = avatar->FindComponentByClass<UMWCharacterAnimControlComponent>())
				{
					animControlComp->StartApproachPoint(OriginalPosition);
				}

				if (PlayMontage(returnApproachAnim, ReturnAnimBlendingOutDelegate, ReturnAnimEndedDelegate))
				{
					SetApproachState(ComboStep, true);

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
}

void UMWBattleUnitAttack::OnApproachAnimEnded(UAnimMontage* Montage, bool bInterrupted)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, FString::Printf(TEXT("UMWBattleUnitAttack::OnApproachAnimEnded: Montage[%s] Interrupted[%s]"), *GetNameSafe(Montage), bInterrupted ? TEXT("True") : TEXT("False")));

	const FMWCharacterBattleSkillGroup* skillGroup = SkillTable.GetComboAt(ComboStep);

	if (!skillGroup)
	{
		UE_LOG(LogMWBattle, Warning, TEXT("UMWBattleUnitAttack::GameplayEventCallback: Skill group not found for combo step[%d]"), ComboStep);

		return;
	}

	const FMWCharacterBattleSkillData* skillData = skillGroup->GetSkill(ComboType);

	if (!skillData)
	{
		return;
	}

	UAnimMontage* skillAnim = skillData->Animation;

	PlaySkillAnim(skillAnim);

	SetApproachState(ComboStep, false);
}

void UMWBattleUnitAttack::OnReturnAnimBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
}

void UMWBattleUnitAttack::OnReturnAnimEnded(UAnimMontage* Montage, bool bInterrupted)
{
	ResetApproachStates();

	SetOriginalPositionFlag(false);

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
	if (!bInterrupted && ComboStep >= SkillTable.Num())
	{
		OnComboEnd();
	}

	// TODO : Check avatar's action point or any other conditions to decide whether to end combo.
}

void UMWBattleUnitAttack::SetupSkillTable(AActor* AvatarActor)
{
	// If your avatar exposes a skill table, cast and read it:
	if (AvatarActor)
	{
		if (UMWBattleUnitComponent* battleUnitComp = AvatarActor->FindComponentByClass<UMWBattleUnitComponent>())
		{
			if (auto* table = battleUnitComp->GetSkillData())
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
				UE_LOG(LogMWBattle, Warning, TEXT("UMWBattleUnitAttack::SetupSkillTable: SkillTable is null in Avatar=%s"), *AvatarActor->GetName());
			}
		}
	}
}

void UMWBattleUnitAttack::ResetCombo()
{
	ComboStep = 0;
}

void UMWBattleUnitAttack::BindDelegates()
{
	ComboEventHandle = GetAbilitySystemComponentFromActorInfo_Ensured()->GenericGameplayEventCallbacks.FindOrAdd(ComboTag).AddUObject(this, &ThisClass::OnCombo);
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
	}
}

bool UMWBattleUnitAttack::PlayMontage(UAnimMontage* InMontage, FOnMontageBlendingOutStarted& BlendingOutDelegate, FOnMontageEnded& EndDelegate)
{
	if (AMWBattleUnitAvatar* avatar = Cast<AMWBattleUnitAvatar>(GetAvatarActorFromActorInfo()))
	{
		if (USkeletalMeshComponent* skMeshComp = avatar->GetComponentByClass<USkeletalMeshComponent>())
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
	return ApproachStates[ComboStep];
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
			++ComboStep;

			return true;
		}
	}
	else
	{
		UE_LOG(LogMWBattle, Warning, TEXT("UMWBattleUnitAttack::GameplayEventCallback: Skill animation not found for combo type[%d] step[%d]"), (int)ComboType, ComboStep);
	}

	return false;
}
UE_ENABLE_OPTIMIZATION