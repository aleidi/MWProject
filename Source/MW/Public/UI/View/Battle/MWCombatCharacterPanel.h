#pragma once

// Include Header
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Messaging/UI/MWUIMessages.h"
#include "UI/MWUserWidget.h"
#include "MWCombatCharacterPanel.generated.h"

// Forward Declare
class UMWCharacterBaseInfo;
class UMWCharacterSkillInfo;
class UMWVMCharacterStatus;
class UMWVMCharacterGroup;
class UUniformGridPanel;

// Define

/*
 * @class UMWCombatCharacterPanel
 * 
 * @brief
 *
 * @note
 */
UCLASS()
class UMWCombatCharacterPanel : public UMWUserWidget
{
	GENERATED_BODY()

public:
public:
	UFUNCTION(BlueprintCallable, Category = "MW|CombatCharacterPanel")
	void SetCharacterGroupViewModel(UMWVMCharacterGroup* InCharacterGroupViewModel);

protected:
	virtual void NativeConstruct() override;

private:
	void BindCharacterViewModels();

	void HandleCharacterStatusMessage(FGameplayTag Channel, const FMWCharacterStatusMessage& Message);

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UMWCharacterBaseInfo> WBP_MainCharacter;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UMWCharacterBaseInfo> WBP_SubCharacter;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> UGP_BuffList;

	// ===== ViewModel =====
	UPROPERTY()
	TObjectPtr<UMWVMCharacterGroup> VMCharacterGroup;

	UPROPERTY()
	TObjectPtr<UMWVMCharacterStatus> VMMainCharacter;

	UPROPERTY()
	TObjectPtr<UMWVMCharacterStatus> VMSubCharacter;

	//==== Gameplay Message ====
	UPROPERTY(BlueprintReadWrite)
	int32 CharacterId = INDEX_NONE;

	FGameplayMessageListenerHandle CharacterStatusListenerHandle;
};
