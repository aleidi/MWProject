#pragma once

// ヘッダーをインクルード
#include "MW.h"
#include "UI/ViewModel/MWViewModelBase.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "GameplayTagContainer.h"
#include "MWVMCombatUI.generated.h"

// 前方宣言
class UMWVMCharacterGroup;
class UMWVMSkillBar;
struct FMWSkillChangedMessage;

// 定義

/*
 * @class UMWVMCombatUI
 * 
 * @brief 使用中のViewModelを集約するContainerです。
 *        Binding処理は行いません。
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
	//==== 左スキルバー ====
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	TObjectPtr<UMWVMSkillBar> VMLeftSkillBar;

	//==== 右スキルバー ====
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	TObjectPtr<UMWVMSkillBar> VMRightSkillBar;

	//==== キャラクター情報 ====
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	TArray<TObjectPtr<UMWVMCharacterGroup>> VMCharacterStatusList;

	FGameplayMessageListenerHandle SkillChangedListenerHandle;
};
