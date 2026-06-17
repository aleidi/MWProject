#pragma once

#include "AbilitySystemInterface.h"
#include "Define/MWDefineGameplay.h"
#include "GameFramework/Character.h"
#include "Interface/MWSelectableInterface.h"
#include "ModularCharacter.h"
#include "MWCharacter.generated.h"

class UCameraComponent;
class UC3DCameraComponent;
class UMWAppearanceComponent;
class UMWPawnExtensionComponent;
struct FMWAbilitySet_GrantedHandles;

UCLASS()
class MW_API AMWCharacter : public AModularCharacter,
							public IMWSelectableInterface,
							public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AMWCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void Tick(float DeltaSeconds) override;
	
	// Only called on the Server. Calls before Server's AcknowledgePossession.
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void PostInitializeComponents() override;

public:
	USkeletalMeshComponent* GetDummyMesh() const;
	USkeletalMeshComponent* GetVisualMesh() const;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> VisualMesh;

#pragma region GAS
public:
	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(Blueprintpure, Category = "MWCharacter|GAS")
	UMWAbilitySystemComponent* GetMWAbilitySystemComponent() const;

protected:
	void SetupDefaultAbilities();

	/** 命中已加载的 PDA 后真正授予 ability。 */
	void ApplyDefaultAbilitiesFromPrimaryData(class UMWCharacterPrimaryData* PrimaryData);

protected:
	// The ability system component sub-object used by player characters.
	UPROPERTY(Category=Camera, VisibleAnywhere, BlueprintReadOnly)
	UMWAbilitySystemComponent* AbilitySystemComponent;

	/** Cache the granted abilities */
	TSharedPtr<FMWAbilitySet_GrantedHandles> AbilityGranetedHandles;
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

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UMWPawnExtensionComponent> ExtensionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UMWAppearanceComponent> AppearanceComp;

public:
	virtual FVector GetFloorLocation() const;

	void SetCharacterLocation(FVector FloorLocation);
};
