#pragma once

#include "GameFramework/PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "MWPlayerController.generated.h"

class FMWTargetSelector;
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
public:
	/* Add a new mapping context to input system. */
	UFUNCTION(BlueprintCallable, Category = "PlayerController|Input")
	bool AddNewMappingContext(const FName& Tag);

	/* Remove the last added mapping context from input system. */
	UFUNCTION(BlueprintCallable, Category = "PlayerController|Input")
	bool RemoveLastMappingContext();


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

	// Battle Command
	void Input_CMD_Attack(const FInputActionValue& InputActionValue);
	void Input_CMD_ChangeLeader(const FInputActionValue& InputActionValue);
	void Input_CMD_Charge(const FInputActionValue& InputActionValue);
	void Input_CMD_Item(const FInputActionValue& InputActionValue);
	void Input_CMD_Special(const FInputActionValue& InputActionValue);
	void Input_CMD_Spirit(const FInputActionValue& InputActionValue);

	// Combat Command
	void Input_CC_Attack(const FInputActionValue& InputActionValue);
	void Input_CC_Direction(const FInputActionValue& InputActionValue);
	void Input_CC_SupportAttack1(const FInputActionValue& InputActionValue);
	void Input_CC_SupportAttack2(const FInputActionValue& InputActionValue);
	void Input_CC_UltimateSkill(const FInputActionValue& InputActionValue);

protected:
	UPROPERTY(EditAnywhere, Category = "Input", meta = (DisplayName="IMC"))
	FName IMCTag = TEXT("BaseInput");

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
private:

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