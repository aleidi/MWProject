#pragma once

#include "GameFramework/PlayerController.h"
#include "MWPlayerController.generated.h"

class FMWTargetSelector;
class UAbilitySystemComponent;
class UMWAbilitySystemComponent;

UCLASS()
class AMWPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMWPlayerController();

protected:
	class AMWCharacter* GetMWCharacter() const;

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
	UFUNCTION()
	void NotifyPlayerAttackted(FMWFoundActorInfo Attacker, FMWFoundActorInfo Attackee);
	UFUNCTION()
	void OnOtherCharacterDied(FMWFoundActorInfo InCharacter);
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