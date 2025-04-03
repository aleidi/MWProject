#pragma once

#include "GameplayAbility/MWGameplayAbility.h"
#include "MWSkillBase.generated.h"

/* UMWSkillBase
* 
* The base class of skill cast.
*/
UCLASS()
class MW_API UMWSkillBase : public UMWGameplayAbility
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill")
	TObjectPtr<UAnimMontage> SkillAnim = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill")
	float Rate = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill")
	FName StartSection = TEXT("");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill")
	float StartTimeSeconds = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill")
	float AnimRootMotionTranslationScale = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill")
	bool bAllowInterruptAfterBlendOut = false;

protected:
	/* This function is called just before PlayMontage is called.*/
	virtual void ProcessBeforeMontagePlay() {}
	/* This function is called just after PlayMontage is called. It's not callback for montage is completed. Use "OnMontageCompleted" instead. */
	virtual void ProcessAfterMontagePlay() {}

	virtual void PlayMontage();

	/* Callback for montage that is ended. */
	void OnMontageEnded(UAnimMontage* Anim, bool IsInterrupted);
	/* Callback for montage that is blending out. */
	void OnMontageBlendingOut(UAnimMontage* Anim, bool IsInterrupted);

	/* Override this function to do process when montage is interrupted. */
	virtual void OnMontageInterrupted() {}
	/* Override this function to do process when montage is interrupted. */
	virtual void OnMontageCompleted() {}
	/* Override this function to do process when montage is blended out. */
	virtual void OnMontageBlendOut() {}

	/* Called when a montage hits a 'PlayMontageNotify' or 'PlayMontageNotifyWindow' begin.
	*  This is useful for processing some extra logic when montage is playing.
	*/
	virtual void OnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload) {}

	/* Called when a montage hits a 'PlayMontageNotify' or 'PlayMontageNotifyWindow' end.
	*  This is useful for processing some extra logic when montage is playing.
	*/
	virtual void OnMontageNotifyEnd(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload) {}

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	/* Add OnAbilityCancelled before OnGameplayAbilityCancelled is broadcast. */
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;
	
	virtual void OnAbilityCancelled();

	/** Checks if the ability is playing a montage and stops that montage, returns true if a montage was stopped, false if not. */
	bool StopPlayingMontage();

private:
	FOnMontageBlendingOutStarted BlendingOutDelegate;
	FOnMontageEnded MontageEndedDelegate;
	FDelegateHandle InterruptedHandle;

	UPROPERTY()
	FActiveGameplayEffectHandle BlockCastSkillEffectHandle;
};