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

protected:
	class AMWCharacter* GetMWCharacter() const;

#pragma region Select Target
public:
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


#pragma region Battle
private:
	UFUNCTION()
	void NotifyPlayerAttackted(FMWFoundActorInfo Attacker, FMWFoundActorInfo Attackee);
	UFUNCTION()
	void OnOtherCharacterDied(FMWFoundActorInfo InCharacter);
#pragma endregion

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void BindDelegates();

private:

};