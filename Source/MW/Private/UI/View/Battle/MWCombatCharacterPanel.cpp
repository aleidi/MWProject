#include "UI/View/Battle/MWCombatCharacterPanel.h"

#include "UI/View/Battle/MWCharacterBaseInfo.h"
#include "UI/ViewModel/Battle/MWVMCharacterGroup.h"
#include "UI/ViewModel/Battle/MWVMCharacterStatus.h"

void UMWCombatCharacterPanel::SetCharacterGroupViewModel(UMWVMCharacterGroup* InCharacterGroupViewModel)
{
	VMCharacterGroup = InCharacterGroupViewModel;

	BindCharacterViewModels();
}

void UMWCombatCharacterPanel::NativeConstruct()
{
	Super::NativeConstruct();

	if (WBP_MainCharacter)
	{
		VMMainCharacter = WBP_MainCharacter->CreateAndSetViewModelByClass<UMWVMCharacterStatus>();
	}

	if( WBP_SubCharacter)
	{
		VMSubCharacter = WBP_SubCharacter->CreateAndSetViewModelByClass<UMWVMCharacterStatus>();
	}


	CharacterStatusListenerHandle = UGameplayMessageSubsystem::Get(this).RegisterListener<FMWCharacterStatusMessage>(
		FGameplayTag(), // TODO: キャラクターステータスメッセージ専用タグを使用
		this,
		&UMWCombatCharacterPanel::HandleCharacterStatusMessage);
	

	if (UWorld* world = GetWorld())
	{
		FTimerDelegate timerDele = FTimerDelegate::CreateWeakLambda(this, [this]()
			{
				if (VMMainCharacter)
				{
					VMMainCharacter->SetPlayerName(FText::FromString(TEXT("Main Character")));
					VMMainCharacter->SetLevel(12);
					VMMainCharacter->SetMaxHealth(1000);
					VMMainCharacter->SetCurrentHealth(780);
				}

				if (VMSubCharacter)
				{
					VMSubCharacter->SetPlayerName(FText::FromString(TEXT("Sub Character")));
					VMSubCharacter->SetLevel(9);
					VMSubCharacter->SetMaxHealth(820);
					VMSubCharacter->SetCurrentHealth(610);
				}
			});
		FTimerHandle handle;
		world->GetTimerManager().SetTimer(handle, timerDele, 4.0f, false);
	}
}

void UMWCombatCharacterPanel::BindCharacterViewModels()
{
	if (!VMCharacterGroup)
	{
		return;
	}

	VMMainCharacter = VMCharacterGroup->GetMainCharacter();
	VMSubCharacter = VMCharacterGroup->GetSubCharacter();

	if (WBP_MainCharacter && VMMainCharacter)
	{
		const FName vmName = WBP_MainCharacter->GetManualViewModelNameByClass(UMWVMCharacterStatus::StaticClass());
		if (!vmName.IsNone())
		{
			WBP_MainCharacter->SetViewModel(vmName, VMMainCharacter, true);
		}
	}

	if (WBP_SubCharacter && VMSubCharacter)
	{
		const FName vmName = WBP_SubCharacter->GetManualViewModelNameByClass(UMWVMCharacterStatus::StaticClass());
		if (!vmName.IsNone())
		{
			WBP_SubCharacter->SetViewModel(vmName, VMSubCharacter, true);
		}
	}
}

void UMWCombatCharacterPanel::HandleCharacterStatusMessage(FGameplayTag Channel, const FMWCharacterStatusMessage& Message)
{
	if (Message.CharacterId != CharacterId)
	{
		return;
	}

	auto applyStatus = [](UMWVMCharacterStatus* vm, const FMWCharacterStatusData& statusData)
	{
		if (!vm)
		{
			return;
		}

		vm->SetPlayerName(statusData.PlayerName);
		vm->SetCurrentHealth(statusData.CurrentHealth);
		vm->SetMaxHealth(statusData.MaxHealth);
		vm->SetLevel(statusData.Level);
		vm->SetPortrait(statusData.Portrait);
	};

	applyStatus(VMMainCharacter.Get(), Message.MainCharacter);
	applyStatus(VMSubCharacter.Get(), Message.SubCharacter);
}
