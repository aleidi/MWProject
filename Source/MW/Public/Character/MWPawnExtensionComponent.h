#pragma once

#include "Gameplay/MWPawnComponent.h"
#include "MWPawnExtensionComponent.generated.h"

class UMWPawnData;
class UMWAbilitySystemComponent;
class FMWTargetSelector;
struct FMWAbilitySetGrantedHandles;
struct FMWFoundActorInfo;

UCLASS(Blueprintable, Meta=(BlueprintSpawnableComponent))
class UMWPawnExtensionComponent : public UMWPawnComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	UFUNCTION(BlueprintPure, Category = "Pawn|Extension")
	static UMWPawnExtensionComponent* FindPawnExtensionComponent(const AActor* Actor);

#pragma region PawnSetting
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setting")
	FName PawnDataName;
#pragma endregion

#pragma region Ability
public:
	void InitializeAbilitySystem(UMWAbilitySystemComponent* InASC, AActor* OnwnerActor);
	void UninitializeAbilitySystem();

protected:
	void GiveAbility();

public:
	/** Delegate fired when our pawn becomes the ability system's avatar actor */
	FSimpleMulticastDelegate OnAbilitySystemInitialized;

	/** Delegate fired when our pawn is removed as the ability system's avatar actor */
	FSimpleMulticastDelegate OnAbilitySystemUninitialized;
protected:
	UPROPERTY()
	TObjectPtr<UMWAbilitySystemComponent> AbilitySystemComponent;

	/* Cache the granted abilities */
	TUniquePtr<FMWAbilitySetGrantedHandles> AbilityGranetedHandles;
#pragma endregion

#pragma region Select
public:
	void InitializeTargetSelector(const AController* InControler);

	void SwitchToLeftTarget();
	void SwitchToRightTarget();
	void CancelSelect();
	void LockTarget();
	void UnlockTarget();
	void ForceLockIfNoTarget(const FMWFoundActorInfo& Target);
	void OnTargetNotExisted(const FMWFoundActorInfo& Target);
protected:
	TUniquePtr<FMWTargetSelector> TargetSelector;
#pragma endregion
};