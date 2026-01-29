#include "Component/Skill/MWSkillLoadoutComponent.h"

UMWSkillLoadoutComponent::UMWSkillLoadoutComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UMWSkillLoadoutComponent::BeginPlay()
{
	Super::BeginPlay();
}

bool UMWSkillLoadoutComponent::SetSkillInSlot(EMWSkillLoadoutSlot Slot, int32 SkillId)
{
	if(!SkillSlots.IsValidIndex((int32)Slot))
	{
		return false;
	}

	SkillSlots[(int32)Slot].Id = SkillId;

	return true;
}

void UMWSkillLoadoutComponent::Init()
{
	if (SkillSlots.Num() != (int32)EMWSkillLoadoutSlot::Max)
	{
		SkillSlots.SetNum((int32)EMWSkillLoadoutSlot::Max);

		for (int32 i = 0; i < (int32)EMWSkillLoadoutSlot::Max; ++i)
		{
			SkillSlots[i].Slot = (EMWSkillLoadoutSlot)i;
		}
	}
}
