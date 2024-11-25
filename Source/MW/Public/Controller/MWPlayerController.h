#pragma once

#include "GameFramework/PlayerController.h"
#include "MWPlayerController.generated.h"

class FMWTargetSelector;

UCLASS()
class AMWPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMWPlayerController();

#pragma region Select Target
	UFUNCTION(BlueprintCallable, Category="TargetSelect")
	void SwitchToLeft();
	UFUNCTION(BlueprintCallable, Category="TargetSelect")
	void SwitchToRight();
	UFUNCTION(BlueprintCallable, Category="TargetSelect")
	void CancelSelect();
	UFUNCTION(BlueprintCallable, Category="TargetSelect")
	void LockTarget();
	UFUNCTION(BlueprintCallable, Category="TargetSelect")
	void UnlockTarget();
#pragma endregion
private:
#pragma region Battle
	UFUNCTION()
	void NotifyPlayerAttackted(FMWActorInfo Attacker, FMWActorInfo Attackee);
	UFUNCTION()
	void OnOtherCharacterDied(FMWActorInfo InCharacter);
#pragma endregion

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void BindDelegates();

private:
	TSharedPtr<FMWTargetSelector> TargetSearcher;

};