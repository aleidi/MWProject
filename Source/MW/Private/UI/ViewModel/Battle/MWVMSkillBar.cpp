#include "UI/ViewModel/Battle/MWVMSkillBar.h"

#include "Messaging/UI/MWUIMessages.h"
#include "MWLogChannels.h"
#include "UI/ViewModel/Battle/MWVMCharacterSkill.h"

UMWVMSkillBar::UMWVMSkillBar()
{
}

UMWVMCharacterSkill* UMWVMSkillBar::GetSkillByIndex(int32 Index) const
{
	switch (Index)
	{
	case MWSkillBarSlot::Up:
		return VMSkillUp.Get();
	case MWSkillBarSlot::Left:
		return VMSkillLeft.Get();
	case MWSkillBarSlot::Down:
		return VMSkillDown.Get();
	case MWSkillBarSlot::Right:
		return VMSkillRight.Get();
	default:
		UE_LOG(LogMWUI, Warning, TEXT("GetSkillByIndex: Index %d is over range [0-%d]"), Index, MWSkillBarSlot::Count - 1);
		break;
	}

	return nullptr;
}

bool UMWVMSkillBar::ApplySkillChanged(const FMWSkillChangedMessage& Message)
{
	UMWVMCharacterSkill* skillVM = GetSkillByIndex(Message.SlotIndex);
	if (!skillVM)
	{
		return false;
	}

	skillVM->SetSkillName(Message.SkillName);
	skillVM->SetSkillIcon(Message.SkillIcon);
	skillVM->SetRemainUse(Message.RemainUse);
	skillVM->SetCurrentRecoveryAmount(Message.CurrentRecoveryAmount);
	skillVM->SetMaxRecoveryAmount(Message.MaxRecoveryAmount);

	return true;
}

void UMWVMSkillBar::OnInitialize()
{
	if (!VMSkillUp)
	{
		VMSkillUp = NewObject<UMWVMCharacterSkill>(this);
	}

	if (!VMSkillLeft)
	{
		VMSkillLeft = NewObject<UMWVMCharacterSkill>(this);
	}

	if (!VMSkillDown)
	{
		VMSkillDown = NewObject<UMWVMCharacterSkill>(this);
	}

	if (!VMSkillRight)
	{
		VMSkillRight = NewObject<UMWVMCharacterSkill>(this);
	}

	UObject* worldContextObject = GetWorldContextObject();

	if (VMSkillUp && worldContextObject)
	{
		VMSkillUp->Initialize(worldContextObject);
	}

	if (VMSkillLeft && worldContextObject)
	{
		VMSkillLeft->Initialize(worldContextObject);
	}

	if (VMSkillDown && worldContextObject)
	{
		VMSkillDown->Initialize(worldContextObject);
	}

	if (VMSkillRight && worldContextObject)
	{
		VMSkillRight->Initialize(worldContextObject);
	}
}

void UMWVMSkillBar::OnDeinitialize()
{
	if (VMSkillUp)
	{
		VMSkillUp->Deinitialize();
	}

	if (VMSkillLeft)
	{
		VMSkillLeft->Deinitialize();
	}

	if (VMSkillDown)
	{
		VMSkillDown->Deinitialize();
	}

	if (VMSkillRight)
	{
		VMSkillRight->Deinitialize();
	}
}
