#include "GameplayAbility/Battle/Skill/MWSkillSystem.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "GameplayAbility/Battle/Skill/MWSkillBase.h"
#include "Data/MWGlobalData.h"
#include "System/MWAssetManager.h"
#include "MWLogChannels.h"

UMWSkillSystem::UMWSkillSystem(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UMWSkillSystem::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	return;
}

void UMWSkillSystem::GrantSkill(const FSkillSpec& SkillSpec)
{
	OwnedSkill.AddUnique(SkillSpec);
}

const TArray<FSkillSpec>& UMWSkillSystem::GetGrantedSkill() const
{
	return OwnedSkill;
}

void UMWSkillSystem::EquipSkill(const FSkillGroupNo& No, const FSkillSpec& NewSkillSpec)
{
	if (!OwnedSkill.Contains(NewSkillSpec))
	{
		UE_LOG(LogMWSkill, Warning, TEXT("Skill [%s] to be equipped is not owned!"), *NewSkillSpec.ToString());
	}

	FSkillSpec& spec = EquippedSkill.FindOrAdd(No);
	if (spec != NewSkillSpec)
	{
		UE_LOG(LogMWSkill, Log, TEXT("%s of [%s] is changed to %s."), *spec.ToString(), *No.ToString(), *NewSkillSpec.ToString());
		spec = NewSkillSpec;
	}
}

const TMap<FSkillGroupNo, FSkillSpec>& UMWSkillSystem::GetSetSkill() const
{
	return EquippedSkill;
}

void UMWSkillSystem::CastSkill(const FSkillGroupNo& No)
{
	const FSkillSpec* spec = EquippedSkill.Find(No);
	if (spec == nullptr)
	{
		UE_LOG(LogMWSkill, Warning, TEXT("Cast skill failed because the %s skill group is not exist."), *No.ToString());
		return;
	}

	if (UAbilitySystemComponent* asc = GetAbilitySystemComponentFromActorInfo())
	{
		asc->TryActivateAbilityByClass(spec->SkillAbility);
	}
	else
	{
		UE_LOG(LogMWSkill, Warning, TEXT("AbilitySystemComponent of ability [%s] is not existed."), *GetName());
	}
}

void UMWSkillSystem::CastSkill(const FSkillSpec& Skill)
{
	if (!OwnedSkill.Contains(Skill))
	{
		UE_LOG(LogMWSkill, Warning, TEXT("Skill [%s] to cast is not owned!"), *Skill.ToString());
		return;
	}

	if (UAbilitySystemComponent* asc = GetAbilitySystemComponentFromActorInfo())
	{
		asc->TryActivateAbilityByClass(Skill.SkillAbility);
	}
	else
	{
		UE_LOG(LogMWSkill, Warning, TEXT("AbilitySystemComponent of ability [%s] is not existed."), *GetName());
	}
}

void UMWSkillSystem::CastSkill(const FSkillGroupNo& No, const FSkillSpec& Skill, bool bForce)
{
	if (bForce)
	{
		CastSkill(Skill);
	}
	else
	{
		CastSkill(No);
	}
}
