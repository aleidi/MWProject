#pragma once

// Include Header
#include "MW.h"
#include "UI/ViewModel/MWViewModelBase.h"
#include "MWVMSkillBar.generated.h"

// Forward Declare
class UMWVMCharacterSkill;
struct FMWSkillChangedMessage;

// Define
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
 * @brief This class is used only as an aggregation container for the ViewModels that are actually in use.
 *        It does not perform any binding
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
