#pragma once

// ヘッダーをインクルード
#include "MW.h"
#include "UI/ViewModel/MWViewModelBase.h"
#include "MWVMSkillBar.generated.h"

// 前方宣言
class UMWVMCharacterSkill;
struct FMWSkillChangedMessage;

// 定義
namespace MWSkillBarSlot
{
	static constexpr int32 Up = 0;
	static constexpr int32 Left = 1;
	static constexpr int32 Down = 2;
	static constexpr int32 Right = 3;
	static constexpr int32 Count = 4;
}

/*
 * @class UMWVMSkillBar
 * 
 * @brief 使用中のViewModelを集約するContainerです。
 *        Binding処理は行いません。
 *
 * @note
 */
UCLASS()
class UMWVMSkillBar : public UMWViewModelBase
{
	GENERATED_BODY()

public:
	UMWVMSkillBar();

	UFUNCTION(BlueprintPure, Category = "MW|MVVM")
	UMWVMCharacterSkill* GetSkillByIndex(int32 Index) const;

	bool ApplySkillChanged(const FMWSkillChangedMessage& Message);

protected:
	virtual void OnInitialize() override;

	virtual void OnDeinitialize() override;

private:
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	TObjectPtr<UMWVMCharacterSkill> VMSkillUp;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	TObjectPtr<UMWVMCharacterSkill> VMSkillLeft;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	TObjectPtr<UMWVMCharacterSkill> VMSkillDown;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	TObjectPtr<UMWVMCharacterSkill> VMSkillRight;
};
