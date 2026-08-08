#pragma once

// Include Header
#include "MW.h"
#include "UI/ViewModel/MWViewModelBase.h"
#include "MWVMCharacterGroup.generated.h"

// Forward Declare
class UMWVMCharacterStatus;

// Define

/*
 * @class UMWVMCharacterGroup
 * 
 * @brief This class is used only as an aggregation container for the ViewModels that are actually in use.
 *        It does not perform any binding
 *
 * @note
 */
UCLASS()
class UMWVMCharacterGroup : public UMWViewModelBase
{
	GENERATED_BODY()
	
public:
	UMWVMCharacterGroup();

	UFUNCTION(BlueprintPure, Category = "MW|MVVM")
	UMWVMCharacterStatus* GetMainCharacter() const;

	UFUNCTION(BlueprintPure, Category = "MW|MVVM")
	UMWVMCharacterStatus* GetSubCharacter() const;

protected:
	virtual void OnInitialize() override;

	virtual void OnDeinitialize() override;

private:
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	TObjectPtr<UMWVMCharacterStatus> VMMainCharacter = nullptr;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	TObjectPtr<UMWVMCharacterStatus> VMSubCharacter = nullptr;
};
