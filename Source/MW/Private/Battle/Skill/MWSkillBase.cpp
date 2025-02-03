#include "Battle/Skill/MWSkillBase.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "MWLogChannels.h"
#include "Data/MWGlobalData.h"
#include "System/MWAssetManager.h"

void UMWSkillBase::PlayMontage()
{
	check(SkillAnim);

	bool bPlayedMontage = false;

	if (UAbilitySystemComponent* asc = GetAbilitySystemComponentFromActorInfo())
	{
		const FGameplayAbilityActorInfo* ActorInfo = GetCurrentActorInfo();
		UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			if (asc->PlayMontage(this, GetCurrentActivationInfo(), SkillAnim, Rate, StartSection, StartTimeSeconds) > 0.f)
			{
				BlendingOutDelegate.BindUObject(this, &UMWSkillBase::OnMontageBlendingOut);
				AnimInstance->Montage_SetBlendingOutDelegate(BlendingOutDelegate, SkillAnim);

				MontageEndedDelegate.BindUObject(this, &UMWSkillBase::OnMontageEnded);
				AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, SkillAnim);

				ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());
				if (Character && (Character->GetLocalRole() == ROLE_Authority ||
					(Character->GetLocalRole() == ROLE_AutonomousProxy && GetNetExecutionPolicy() == EGameplayAbilityNetExecutionPolicy::LocalPredicted)))
				{
					Character->SetAnimRootMotionTranslationScale(AnimRootMotionTranslationScale);
				}

				bPlayedMontage = true;

				// Block Cast Skill at the begining of the montage is playing
				TSubclassOf<UGameplayEffect> blockcast = UMWAssetManager::Get().GetSubclass(UMWGlobalData::Get().GEBlockCastSkill);
				FGameplayEffectSpecHandle spec_handle = asc->MakeOutgoingSpec(blockcast, 1.f, asc->MakeEffectContext());
				BlockCastSkillEffectHandle = asc->ApplyGameplayEffectSpecToSelf(*spec_handle.Data.Get());
			}
		}
		else
		{
			UE_LOG(LogMWAbilitySystem, Warning, TEXT("%s call to PlayMontage failed!"), *GetName());
		}
	}
	else
	{
		UE_LOG(LogMWAbilitySystem, Warning, TEXT("%s called on invalid AbilitySystemComponent"), *GetName());
	}

	if (!bPlayedMontage)
	{
		UE_LOG(LogMWAbilitySystem, Warning, TEXT("Skil Ability [%s] failed to play montage %s"), *GetName(), *GetNameSafe(SkillAnim));
		OnAbilityCancelled();
	}
}

void UMWSkillBase::OnMontageEnded(UAnimMontage* Anim, bool IsInterrupted)
{
	if (!IsInterrupted)
	{
		OnMontageCompleted();
	}

	if (BlockCastSkillEffectHandle.IsValid() && Anim == SkillAnim)
	{
		// if effect is applied, then remove it at the end of the montage
		if (UAbilitySystemComponent* asc = GetAbilitySystemComponentFromActorInfo())
		{
			asc->RemoveActiveGameplayEffect(BlockCastSkillEffectHandle);
		}
	}

}

void UMWSkillBase::OnMontageBlendingOut(UAnimMontage* Anim, bool IsInterrupted)
{
	const bool is_playing_skill_anim = (Anim == SkillAnim) && (GetCurrentMontage() == SkillAnim);
	if (is_playing_skill_anim)
	{
		// Reset AnimRootMotionTranslationScale
		ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());
		if (Character && (Character->GetLocalRole() == ROLE_Authority ||
			(Character->GetLocalRole() == ROLE_AutonomousProxy && GetNetExecutionPolicy() == EGameplayAbilityNetExecutionPolicy::LocalPredicted)))
		{
			Character->SetAnimRootMotionTranslationScale(1.f);
		}
	}

	if (is_playing_skill_anim && (IsInterrupted || !bAllowInterruptAfterBlendOut))
	{
		if (UAbilitySystemComponent* asc = GetAbilitySystemComponentFromActorInfo())
		{
			asc->ClearAnimatingAbility(this);
		}
	}

	if (IsInterrupted)
	{
		OnMontageInterrupted();
	}
	else
	{
		OnMontageBlendOut();
	}
}

void UMWSkillBase::OnAbilityCancelled()
{
	if (StopPlayingMontage() || bAllowInterruptAfterBlendOut)
	{
		OnMontageInterrupted();
	}
}

bool UMWSkillBase::StopPlayingMontage()
{
	const FGameplayAbilityActorInfo* actor_info = GetCurrentActorInfo();
	if (actor_info == nullptr)
	{
		return false;
	}

	UAnimInstance* anim_inst = actor_info->GetAnimInstance();
	if (anim_inst == nullptr)
	{
		return false;
	}

	// Check if the montage is still playing
	// The ability would have been interrupted, in which case we should automatically stop the montage
	UAbilitySystemComponent* asc = GetAbilitySystemComponentFromActorInfo();
	if (asc)
	{
		if (asc->GetAnimatingAbility() == this && asc->GetCurrentMontage() == SkillAnim)
		{
			// Unbind delegates so they don't get called as well
			FAnimMontageInstance* montage_inst = anim_inst->GetActiveInstanceForMontage(SkillAnim);
			if (montage_inst)
			{
				montage_inst->OnMontageBlendingOutStarted.Unbind();
				montage_inst->OnMontageEnded.Unbind();
			}

			asc->CurrentMontageStop();
			return true;
		}
	}

	return false;
}

void UMWSkillBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!IsValid(SkillAnim))
	{
		bool bReplicateEndAbility = true;
		bool bWasCancelled = false;
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
	}

	ProcessBeforeMontagePlay();

	PlayMontage();

	ProcessAfterMontagePlay();
}

void UMWSkillBase::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	if (CanBeCanceled())
	{
		if (ScopeLockCount > 0)
		{
			UE_LOG(LogMWAbilitySystem, Verbose, TEXT("Attempting to cancel Ability %s but ScopeLockCount was greater than 0, adding cancel to the WaitingToExecute Array"), *GetName());
			WaitingToExecute.Add(FPostLockDelegate::CreateUObject(this, &UGameplayAbility::CancelAbility, Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility));
			return;
		}

		// Replicate the the server/client if needed
		if (bReplicateCancelAbility && ActorInfo && ActorInfo->AbilitySystemComponent.IsValid())
		{
			ActorInfo->AbilitySystemComponent->ReplicateEndOrCancelAbility(Handle, ActivationInfo, this, true);
		}
 
		// MW : allow derived class to accept montage interrupt event
		OnAbilityCancelled();

		// Gives the Ability BP a chance to perform custom logic/cleanup when any active ability states are active
		if (OnGameplayAbilityCancelled.IsBound())
		{
			OnGameplayAbilityCancelled.Broadcast();
		}

		// End the ability but don't replicate it, we replicate the CancelAbility call directly
		bool bReplicateEndAbility = false;
		bool bWasCancelled = true;
		EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	}
}
