#include "Battle/MWSkillSystem.h"
#include "Battle/MWBattleTypes.h"
#include "Subsystem/MWBattleSystem.h"

DEFINE_LOG_CATEGORY_STATIC(LogMWSkill, All, All)

FMWSkillGroup::FMWSkillGroup()
{
	Skills.Reserve(MW_SKILL_NUM);
}

TMWSkillSystem::TMWSkillSystem()
	:TMWSkillSystem(nullptr)
{
}

TMWSkillSystem::TMWSkillSystem(AActor* InOwner)
	:Owner(InOwner)
{
	SkillGroups.Reserve(MW_SKILL_GROUP_NUM);
}

bool TMWSkillSystem::TriggerSkill(int32 GroupId, int32 SkillId, TFunction<bool(const FGameplayTag&)> RealTriggerFunc)
{
	if (!Owner)
	{
		return false;
	}

	if (!IsSkillIdValid(GroupId, SkillId))
	{
		return false;
	}

	if (!SkillGroups[GroupId].Skills[SkillId].IsValid())
	{
		UE_LOG(LogMWSkill, Warning, TEXT("Skill %d is not valid!"), SkillId);
		return false;
	}

	if (UMWBattleSystem* mwbs = Owner->GetWorld()->GetSubsystem<UMWBattleSystem>())
	{
		// Trigger skill
		if (RealTriggerFunc(SkillGroups[GroupId].Skills[SkillId].Tag))
		{
			if (mwbs->OnSkillTriggered.IsBound())
			{
				mwbs->OnSkillTriggered.Broadcast(SkillGroups[GroupId].Skills[SkillId].Id);
				return true;
			}
		}

	}

	return false;
}

bool TMWSkillSystem::SetSkill(int32 GroupId, int32 SkillId, const FMWSkill& NewSkill)
{
	if (!IsSkillIdValid(GroupId, SkillId))
	{
		return false;
	}

	SkillGroups[GroupId].Skills[SkillId] = NewSkill;

	return true;
}

bool TMWSkillSystem::IsSkillIdValid(int32 GroupdId, int32 SkillId) const
{
	if (GroupdId + 1 > MW_SKILL_GROUP_NUM)
	{
		UE_LOG(LogMWSkill, Warning, TEXT("GourpId exceeds the range of skill groups"));
		return false;
	}

	if (SkillId + 1 > MW_SKILL_NUM)
	{
		UE_LOG(LogMWSkill, Warning, TEXT("Id exceeds the range of skills"));
		return false;
	}

	return true;
}
