#include "UI/ViewModel/Battle/MWVMCombatUI.h"

#include "Messaging/UI/MWUIMessages.h"
#include "UI/ViewModel/Battle/MWVMCharacterGroup.h"
#include "UI/ViewModel/Battle/MWVMSkillBar.h"

// 定義
#define MAX_CHARACTER_GROUP 6

UMWVMCombatUI::UMWVMCombatUI()
{
}

UMWVMSkillBar* UMWVMCombatUI::GetLeftSkillBar() const
{
	return VMLeftSkillBar;
}

UMWVMSkillBar* UMWVMCombatUI::GetRightSkillBar() const
{
	return VMRightSkillBar;
}

UMWVMSkillBar* UMWVMCombatUI::GetSkillBar(bool bLeftBar) const
{
	return bLeftBar ? VMLeftSkillBar : VMRightSkillBar;
}

const TArray<TObjectPtr<UMWVMCharacterGroup>>& UMWVMCombatUI::GetCharacterStatusList() const
{
	return VMCharacterStatusList;
}

UMWVMCharacterGroup* UMWVMCombatUI::GetCharacterStatus(int32 Index) const
{
	if (!VMCharacterStatusList.IsValidIndex(Index))
	{
		return nullptr;
	}

	return VMCharacterStatusList[Index].Get();
}

bool UMWVMCombatUI::ApplySkillChanged(const FMWSkillChangedMessage& Message)
{
	UMWVMSkillBar* skillBarVM = GetSkillBar(Message.bLeftBar);
	if (!skillBarVM)
	{
		return false;
	}

	return skillBarVM->ApplySkillChanged(Message);
}

void UMWVMCombatUI::OnInitialize()
{
	if (!VMLeftSkillBar)
	{
		VMLeftSkillBar = NewObject<UMWVMSkillBar>(this);
	}

	if (!VMRightSkillBar)
	{
		VMRightSkillBar = NewObject<UMWVMSkillBar>(this);
	}

	UObject* worldContextObject = GetWorldContextObject();

	if (VMLeftSkillBar && worldContextObject)
	{
		VMLeftSkillBar->Initialize(worldContextObject);
	}

	if (VMRightSkillBar && worldContextObject)
	{
		VMRightSkillBar->Initialize(worldContextObject);
	}

	if (VMCharacterStatusList.Num() == 0)
	{
		VMCharacterStatusList.Reserve(MAX_CHARACTER_GROUP);

		UMWVMCharacterGroup* characterStatus = NewObject<UMWVMCharacterGroup>(this);
		if (characterStatus)
		{
			VMCharacterStatusList.Emplace(characterStatus);
		}
	}

	for (const TObjectPtr<UMWVMCharacterGroup>& characterGroup : VMCharacterStatusList)
	{
		if (characterGroup && worldContextObject)
		{
			characterGroup->Initialize(worldContextObject);
		}
	}

	RegisterSkillChangedListener();
}

void UMWVMCombatUI::OnDeinitialize()
{
	UnregisterSkillChangedListener();

	if (VMLeftSkillBar)
	{
		VMLeftSkillBar->Deinitialize();
	}

	if (VMRightSkillBar)
	{
		VMRightSkillBar->Deinitialize();
	}

	for (const TObjectPtr<UMWVMCharacterGroup>& characterGroup : VMCharacterStatusList)
	{
		if (characterGroup)
		{
			characterGroup->Deinitialize();
		}
	}
}

void UMWVMCombatUI::RegisterSkillChangedListener()
{
	if (SkillChangedListenerHandle.IsValid())
	{
		return;
	}

	UObject* worldContextObject = GetWorldContextObject();
	if (!worldContextObject)
	{
		return;
	}

	SkillChangedListenerHandle = UGameplayMessageSubsystem::Get(worldContextObject).RegisterListener<FMWSkillChangedMessage>(
		FGameplayTag(),
		this,
		&UMWVMCombatUI::HandleSkillChangedMessage);
}

void UMWVMCombatUI::UnregisterSkillChangedListener()
{
	if (SkillChangedListenerHandle.IsValid())
	{
		SkillChangedListenerHandle.Unregister();
	}
}

void UMWVMCombatUI::HandleSkillChangedMessage(FGameplayTag Channel, const FMWSkillChangedMessage& Message)
{
	ApplySkillChanged(Message);
}
