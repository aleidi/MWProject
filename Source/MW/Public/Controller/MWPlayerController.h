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
	/** Bind Additional input config.
	 *  @param InputConfig The input config to bind.
	 *  @param OutBindHandles The handles of the binds that were added. These handles can be used to remove the binds later.
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

	// Default Input
	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_LookAt(const FInputActionValue& InputActionValue);
	void Input_LookStick(const FInputActionValue& InputActionValue);
	void Input_Crouch(const FInputActionValue& InputActionValue);
	void Input_AutoRun(const FInputActionValue& InputActionValue);

private:
	TArray<FMWInputMappingContextWithPriority> MappingContextStack;

	FModifyContextOptions MappingOption;

	/** Cache of bind handles for each input config. */
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
	// set AMWPlayerCameraManager to PlayerCameraManager.
	virtual void SpawnPlayerCameraManager() override;

protected:
	// Setup camera components dynamically.
	// カメラコンポーネントの設定.
	void SetupCameraComponents();
#pragma endregion

private:
	UPROPERTY(Transient)
	TObjectPtr<UMWChargeInputProcessor> ChargeInputProcessor = nullptr;
};
