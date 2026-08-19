#pragma once

// ヘッダーをインクルード
#include "Subsystems/WorldSubsystem.h"
#include "InputActionValue.h"
#include "MWWorldSubsystem.generated.h"

// 前方宣言
class AMWInputHandler;

// 定義

/*
 * @class UMWWorldSubsystem
 * 
 * @brief
 *
 * @note
 */
UCLASS()
class UMWWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

	AMWInputHandler* GetInputHandler() const { return InputHandler; }

	bool ShouldCreateSubsystem(UObject* Outer) const override;

private:
	TObjectPtr<AMWInputHandler> InputHandler = nullptr;
};