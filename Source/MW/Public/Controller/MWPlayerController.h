#pragma once

#include "GameFramework/PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "MWPlayerController.generated.h"

class UAbilitySystemComponent;
class UMWAbilitySystemComponent;
class UInputMappingContext;
struct FGameplayTag;
struct FInputActionValue;
struct FMWInputMappingContextWithPriority;

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

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;

	void BindDelegates();

public:
	UFUNCTION(BlueprintPure, Category = "Ability")
	UAbilitySystemComponent* GetAbilitySystemComponent() const;

	UFUNCTION(BlueprintPure, Category = "Ability")
	UMWAbilitySystemComponent* GetMWAbilitySystemComponent() const;

#pragma region Camera
public:
	// set AMWPlayerCameraManager to PlayerCameraManager.
	virtual void SpawnPlayerCameraManager() override;
#pragma endregion
};