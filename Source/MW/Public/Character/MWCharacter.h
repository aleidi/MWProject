#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interface/MWSelectableInterface.h"
#include "Define/MWDefineGameplay.h"
#include "MWCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UMWHeroComponent;
class UMWPawnExtensionComponent;
class UAbilitySystemComponent;
class UMWBattleAttributeSet;

UCLASS()
class MW_API AMWCharacter : public ACharacter,
	public IAbilitySystemInterface,
	public IMWSelectableInterface
{
	GENERATED_BODY()

public:
	AMWCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void Tick(float DeltaSeconds) override;
	
#pragma region GAS
public:
	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(Blueprintpure, Category = "MWCharacter|GAS")
	UMWAbilitySystemComponent* GetMWAbilitySystemComponent() const;

	// Only called on the Server. Calls before Server's AcknowledgePossession.
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;

protected:
	// The ability system component sub-object used by player characters.
	UPROPERTY(Category=Camera, VisibleAnywhere, BlueprintReadOnly)
	UMWAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Ability")
	TObjectPtr<UMWBattleAttributeSet> BattleSet;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability")
	int32 Level;
#pragma endregion 

#pragma region Input
public:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
#pragma endregion

#pragma region Move
private:
	virtual void UpdatePawnRotation(float DeltaTime);

	UFUNCTION(BlueprintCallable, Category = "Move")
	void EnableUpdatePawnRotation(bool bEnabled);
	
private:
	bool CanUpdatePawnRotation() const;

	// temporary value
	FRotator LastVelocityRotation;
	FRotator DesiredPawnRotation;

	UPROPERTY(EditAnywhere, Category = "Pawn")
	float DesiredRotInterpSpeed = 460.f;

	UPROPERTY(EditAnywhere, Category = "Pawn")
	float PawnRotInterpSpeed = 10.f;

	UPROPERTY(EditAnywhere, Category = "Pawn")
	bool bUpdatePawnRotation = false;
#pragma endregion

#pragma region Battle
public:
	UFUNCTION(BlueprintCallable, Category="Battle")
	void ChangeBehaviorState(ECharacterBehaviorState NewState);

protected:
	UFUNCTION(BlueprintPure)
	float BP_GetNormalizedVelocity() const;
	float GetNormalizedVelocity() const;
	bool GetIsMoving() const;
	bool GetCanNormalAtk() const;

protected:
	UPROPERTY(BlueprintReadOnly, Category="Character|Behavior")
	TEnumAsByte<ECharacterBehaviorState> BehaviorState;
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

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Hero")
	TObjectPtr<UMWHeroComponent> HeroComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Hero")
	TObjectPtr<UMWPawnExtensionComponent> ExtensionComp;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void PostInitializeComponents() override;

public:
	UFUNCTION(BlueprintPure)
	int32 GetEntityId() const { return Id; }

protected:
	UPROPERTY()
	int32 Id;
};
