#include "Battle/MWBattleComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogMWBattleComp, All, All)

UMWBattleComponent::UMWBattleComponent()
{
}

bool UMWBattleComponent::TryCastSkill(int32 GroupId, int32 SkillId)
{
	return false;
}

bool UMWBattleComponent::CanCombo() const
{
	return true;
}