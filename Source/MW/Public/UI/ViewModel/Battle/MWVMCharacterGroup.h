#pragma once

// ヘッダーをインクルード
#include "MW.h"
#include "UI/ViewModel/MWViewModelBase.h"
#include "MWVMCharacterGroup.generated.h"

// 前方宣言
class UMWVMCharacterStatus;

// 定義

/*
 * @class UMWVMCharacterGroup
 * 
 * @brief 使用中のViewModelを集約するContainerです。
 *        Binding処理は行いません。
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
