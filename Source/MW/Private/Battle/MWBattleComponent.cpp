#include "Battle/MWBattleComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogMWBattleComp, All, All)

UMWBattleComponent::UMWBattleComponent()
{
	SysSkill = MakeShared<TMWSkillSystem>(this->GetOwner());
}

bool UMWBattleComponent::TryCastSkill(int32 GroupId, int32 SkillId)
{
	if (!SysSkill)
	{
		UE_LOG(LogMWBattleComp, Warning, TEXT("MW Skill System is not instanced!"));
		return false;
	}

	if (auto* as = Cast<IAbilitySystemInterface>(GetOwner()))
	{
		if (auto* asc = as->GetAbilitySystemComponent())
		{
			SysSkill->TriggerSkill(GroupId, SkillId,
				[&](const FGameplayTag& Tag) -> bool 
				{
					return asc->TryActivateAbilitiesByTag(FGameplayTagContainer(Tag));
				}
			);
		}
	}

	return false;
}

bool UMWBattleComponent::CanCombo() const
{
	return true;
}