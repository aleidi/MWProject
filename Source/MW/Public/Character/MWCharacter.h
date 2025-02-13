#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interface/MWSelectableInterface.h"
#include "Interface/MWCharacterInterface.h"
#include "Character/MWCharacterTypes.h"
#include "MWCharacter.generated.h"

class FMWTargetSelector;
class USpringArmComponent;
class UCameraComponent;
class UMWHeroComponent;
class UMWPawnExtensionComponent;
class UAbilitySystemComponent;
class UMWBattleSet;

UCLASS()
class MW_API AMWCharacter : public ACharacter,
	public IAbilitySystemInterface,
	public IMWSelectableInterface,
	public IMWCharacterInterface
{
	GENERATED_BODY()

public:
	AMWCharacter(const FObjectInitializer& ObjectInitializer);
	
#pragma region GAS
public:
	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(Blueprintpure, Category = "MWCharacter|GAS")
	UMWAbilitySystemComponent* GetMWAbilitySystemComponent() const;

	// Only called on the Server. Calls before Server's AcknowledgePossession.
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;

#pragma region Input
public:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
#pragma endregion

protected:
	// The ability system component sub-object used by player characters.
	UPROPERTY(Category=Camera, VisibleAnywhere, BlueprintReadOnly)
	UMWAbilitySystemComponent* AbilitySystemComponent;
#pragma endregion 

#pragma region Battle
public:
	UFUNCTION(BlueprintCallable, Category="Battle")
	void ChangeBattleState(MWBehaviorState::EBehaviorState NewState);

protected:

	UFUNCTION(BlueprintPure)
	float BP_GetNormalizedVelocity() const;
	virtual float GetNormalizedVelocity() const override;
	virtual bool GetIsMoving() const override;
	virtual bool GetCanNormalAtk() const override;

protected:
	UPROPERTY(BlueprintReadOnly, Category="Character|Behavior")
	TEnumAsByte<MWBehaviorState::EBehaviorState> BehaviorState;
	UPROPERTY(BlueprintReadOnly, Category="Character|Battle")
	bool bAttackTarget;
	UPROPERTY(BlueprintReadOnly, Category="Character|Battle")
	bool bAttackTargetInRange;
#pragma endregion
	
#pragma region Camera
protected:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(Category=Camera, VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(Category=Camera, VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCameraComponent> FollowCamera;

#pragma endregion

#pragma region Ability
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Ability")
	TObjectPtr<UMWBattleSet> BattleSet;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability")
	int32 Level;
#pragma endregion

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Hero")
	TObjectPtr<UMWHeroComponent> HeroComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Hero")
	TObjectPtr<UMWPawnExtensionComponent> ExtensionComp;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void PostInitializeComponents() override;

};
