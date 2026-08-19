#pragma once

// ヘッダーをインクルード
#include "MVVMViewModelBase.h"
#include "MWViewModelBase.generated.h"

// 前方宣言

// 定義

/*
 * @class UMWViewModelBase
 * 
 * @brief 
 *
 * @note
 */
UCLASS(Abstract)
class UMWViewModelBase : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	void Initialize(UObject* InWorldContextObject);

	void Deinitialize();

	UFUNCTION(BlueprintPure, Category = "MW|MVVM")
	bool IsInitialized() const;

	UFUNCTION(BlueprintPure, Category = "MW|MVVM")
	UObject* GetWorldContextObject() const;

protected:
	virtual void OnInitialize() {}

	virtual void OnDeinitialize() {}

protected:
	UPROPERTY(Transient)
	TObjectPtr<UObject> WorldContextObject = nullptr;

	bool bInitialized = false;
};
