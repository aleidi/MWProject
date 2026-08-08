#pragma once

// Include Header
#include "MW.h"
#include "UI/ViewModel/MWViewModelBase.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "GameplayTagContainer.h"
#include "MWVMCombatUI.generated.h"

// Forward Declare
class UMWVMCharacterGroup;
class UMWVMSkillBar;
struct FMWSkillChangedMessage;

// Define

/*
 * @class UMWVMCombatUI
 * 
 * @brief This class is used only as an aggregation container for the ViewModels that are actually in use.
 *        It does not perform any binding
 *
 * @note
 */
UCLASS()
class UMWVMCombatUI : public UMWViewModelBase
{
	GENERATED_BODY()

public:
	UMWVMCombatUI();

	UMWVMSkillBar* GetLeftSkillBar() const;

	UMWVMSkillBar* GetRightSkillBar() const;

	UMWVMSkillBar* GetSkillBar(bool bLeftBar) const;

	const TArray<TObjectPtr<UMWVMCharacterGroup>>& GetCharacterStatusList() const;

	UMWVMCharacterGroup* GetCharacterStatus(int32 Index) const;

	bool ApplySkillChanged(const FMWSkillChangedMessage& Message);

protected:
	virtual void OnInitialize() override;

	virtual void OnDeinitialize() override;

private:
	void RegisterSkillChangedListener();

	void UnregisterSkillChangedListener();

	void HandleSkillChangedMessage(FGameplayTag Channel, const FMWSkillChangedMessage& Message);

private:
	//==== Left Skill Bar ====
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	TObjectPtr<UMWVMSkillBar> VMLeftSkillBar;

	//==== Right Skill Bar ====
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	TObjectPtr<UMWVMSkillBar> VMRightSkillBar;

	//==== Character Info ====
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	TArray<TObjectPtr<UMWVMCharacterGroup>> VMCharacterStatusList;

	FGameplayMessageListenerHandle SkillChangedListenerHandle;
};
