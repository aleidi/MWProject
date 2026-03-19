#pragma once

#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"
#include "Input/MWInputConfig.h"
#include "MWPlayerController.generated.h"

class UAbilitySystemComponent;
class UInputMappingContext;
class UMWAbilitySystemComponent;
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
	void AddAdditionalInputConfig(const UMWInputConfig* InputConfig, TArray<uint32>& OutBindHandles);

	void RemoveAdditionalInputConfig(TArray<uint32>& BindHandles);

	UFUNCTION(BlueprintCallable, Category="Input", meta=(DisplayName="Add Additional Input Config"))
	void K2_AddAdditionalInputConfig(UObject* Context, const UMWInputConfig* InputConfig);

	UFUNCTION(BlueprintCallable, Category="Input", meta=(DisplayName="Remove Additional Input Config"))
	void K2_RemoveAdditionalInputConfig(UObject* Context);

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
	TMap<UObject*, TArray<uint32>> BindHandlesCache;

#pragma endregion

#pragma region Select Target
public:
	UFUNCTION(BlueprintCallable, Category="TargetSelect")
	void SwitchToLeftTarget();
	UFUNCTION(BlueprintCallable, Category="TargetSelect")
	void SwitchToRightTarget();
	UFUNCTION(BlueprintCallable, Category="TargetSelect")
	void CancelSelect();
	UFUNCTION(BlueprintCallable, Category="TargetSelect")
	void LockTarget();
	UFUNCTION(BlueprintCallable, Category="TargetSelect")
	void UnlockTarget();
#pragma endregion

#pragma region Battle
public:
	UFUNCTION(BlueprintCallable, Category = "Battle")
	void OnBattleBegin();

	UFUNCTION(BlueprintCallable, Category = "Battle")
	void OnBattleEnd();

private:
	FDelegateHandle DHApplyBattleCommand;
	FDelegateHandle DHRemoveBattleCommand;
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
};