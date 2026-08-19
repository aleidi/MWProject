#pragma once

// ヘッダーをインクルード
#include "InputTriggers.h"
#include "MWInputTriggers.generated.h"

// 前方宣言

// 定義

/*
 * @class UMWInputTriggerNotAction
 * 
 * @brief 
 *
 * @note
 */
UCLASS(NotBlueprintable, meta = (DisplayName = "Trigger Not Action", NotInputConfigurable = "true"))
class UMWInputTriggerNotAction : public UInputTrigger
{
	GENERATED_BODY()
	
protected:
	virtual ETriggerType GetTriggerType_Implementation() const override
	{
		return ETriggerType::Blocker;
	}

	virtual ETriggerState UpdateState_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue ModifiedValue, float DeltaTime) override;

	virtual bool IsBlocking(const ETriggerState State) const override
	{
		// OtherActionに状態がある場合は、現在のアクションをブロックします。
		return State != ETriggerState::None;
	}

public:
	UPROPERTY(EditAnywhere, Category = "Trigger Settings")
	TObjectPtr<const UInputAction> OtherAction = nullptr;
};
