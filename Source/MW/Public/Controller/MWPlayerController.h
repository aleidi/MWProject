#pragma once

#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"
#include "Input/MWInputConfig.h"
#include "MWPlayerController.generated.h"

class UAbilitySystemComponent;
class UInputMappingContext;
class UMWAbilitySystemComponent;
class UMWChargeInputProcessor;
class UMWInputConfig;

struct FGameplayTag;
struct FInputActionValue;

UCLASS()
class AMWPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMWPlayerController();

	virtual void Tick(float DeltaSeconds) override;

protected:
	class AMWCharacter* GetMWCharacter() const;

#pragma region Input
public:
	/** 追加の入力設定をバインドします。
	 *  @param InputConfig バインドする入力設定。
	 *  @param OutBindHandles 追加したバインドのハンドル。後からバインド解除に使用できます。
	 */
	void AddInputConfig(const UMWInputConfig* InputConfig, TArray<uint32>& OutBindHandles);

	void RemoveInputConfig(TArray<uint32>& BindHandles);

	UFUNCTION(BlueprintCallable, Category="Input", meta=(DisplayName="Add Input Config", HidePin = "Context", DefaultToSelf = "Context"))
	void K2_AddInputConfig(AActor* Context, const UMWInputConfig* InputConfig);

	UFUNCTION(BlueprintCallable, Category="Input", meta=(DisplayName="Remove Input Config", HidePin = "Context", DefaultToSelf = "Context"))
	void K2_RemoveInputConfig(AActor* Context);

protected:
	virtual void SetupInputComponent() override;

	void Input_AbilityInputTagPressed(FGameplayTag InputTag);
	void Input_AbilityInputTagReleased(FGameplayTag InputTag);

	// デフォルト入力
	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_LookAt(const FInputActionValue& InputActionValue);
	void Input_LookStick(const FInputActionValue& InputActionValue);
	void Input_Crouch(const FInputActionValue& InputActionValue);
	void Input_AutoRun(const FInputActionValue& InputActionValue);

private:
	TArray<FMWInputMappingContextWithPriority> MappingContextStack;

	FModifyContextOptions MappingOption;

	/** 入力設定ごとのバインドハンドルキャッシュ。 */
	TMap<AActor*, TArray<uint32>> BindHandlesCache;

#pragma endregion

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
	virtual void OnPossess(APawn* aPawn) override;

public:
	UFUNCTION(BlueprintPure, Category = "Ability")
	UAbilitySystemComponent* GetAbilitySystemComponent() const;

	UFUNCTION(BlueprintPure, Category = "Ability")
	UMWAbilitySystemComponent* GetMWAbilitySystemComponent() const;

#pragma region Camera
public:
	// PlayerCameraManagerにAMWPlayerCameraManagerを設定します。
	virtual void SpawnPlayerCameraManager() override;

protected:
	// カメラコンポーネントを動的に設定します。
	// カメラコンポーネントの設定.
	void SetupCameraComponents();
#pragma endregion

private:
	UPROPERTY(Transient)
	TObjectPtr<UMWChargeInputProcessor> ChargeInputProcessor = nullptr;
};
