#include "UI/ViewModel/Battle/MWVMSkillBar.h"

#include "Messaging/UI/MWUIMessages.h"
#include "MWLogChannels.h"
#include "UI/ViewModel/Battle/MWVMCharacterSkill.h"

// Define
namespace
{
	const int32 SkillSlotUp = 0;
	const int32 SkillSlotLeft = 1;
	const int32 SkillSlotDown = 2;
	const int32 SkillSlotRight = 3;
}

UMWVMSkillBar::UMWVMSkillBar()
{
}

UMWVMCharacterSkill* UMWVMSkillBar::GetSkillByIndex(int32 Index) const
{
	switch (Index)
	{
	case SkillSlotUp:
		return VMSkillUp.Get();
	case SkillSlotLeft:
		return VMSkillLeft.Get();
	case SkillSlotDown:
		return VMSkillDown.Get();
	case SkillSlotRight:
		return VMSkillRight.Get();
	default:
		UE_LOG(LogMWUI, Warning, TEXT("GetSkillByIndex:  Index %d is over range [0-3]"), Index);
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
