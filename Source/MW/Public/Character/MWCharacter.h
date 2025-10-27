#pragma once

#include "CoreMinimal.h"
#include "MWCharacterASC.h"
#include "Interface/MWSelectableInterface.h"
#include "Define/MWDefineGameplay.h"
#include "MWCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UMWPawnExtensionComponent;

UCLASS()
class MW_API AMWCharacter : public AMWCharacterASC,
							public IMWSelectableInterface
{
	GENERATED_BODY()

public:
	AMWCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void Tick(float DeltaSeconds) override;
	
	// Only called on the Server. Calls before Server's AcknowledgePossession.
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;

#pragma region Input
public:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
#pragma endregion

#pragma region Animation
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

public:
	UFUNCTION(BlueprintCallable, Category="BDescriptionattle")
	void ChangeBehaviorState(ECharacterBehaviorState NewState);

protected:
	UFUNCTION(BlueprintPure)
	float BP_GetNormalizedVelocity() const;
	float GetNormalizedVelocity() const;
	bool GetIsMoving() const;

protected:
	UPROPERTY(BlueprintReadOnly, Category="Character|Behavior")
	TEnumAsByte<ECharacterBehaviorState> BehaviorState;
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
	TObjectPtr<UMWPawnExtensionComponent> ExtensionComp;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void PostInitializeComponents() override;
};
