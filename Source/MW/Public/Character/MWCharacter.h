#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interface/MWSelectableInterface.h"
#include "Interface/MWCharacterInterface.h"
#include "Character/MWCharacterTypes.h"
#include "MWCharacter.generated.h"

class FMWTargetSelector;
class FMWCameraComponent;

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
protected:
	// Reference to the ASC. It will live on the PlayerState or here if the character doesn't have a PlayerState.
	UPROPERTY()
	class UAbilitySystemComponent* AbilitySystemComponent;

	// Reference to the AttributeSetBase. It will live on the PlayerState or here if the character doesn't have a PlayerState.
	//UPROPERTY()
	//class UXXAttributeSetBase* AttributeSetBase;

public:
	inline virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }
	UFUNCTION(Blueprintpure, Category = "MWCharacter|GAS", meta = (DisplayName = "GetAbilitySystemComponent"))
	UAbilitySystemComponent* K2_GetAbilitySystemComponent() const;
	void InitStartupAbilities();

	// Only called on the Server. Calls before Server's AcknowledgePossession.
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;

protected:
	//UPROPERTY(EditAnywhere, Category = "NWCharacter|GAS")
	//TObjectPtr<class UXXGameplayAbilitySet> StartupAbillities;
#pragma endregion 

#pragma region Battle
public:
	UFUNCTION(BlueprintCallable, Category="Battle")
	void ChangeBattleState(MWBehaviorState::EBehaviorState NewState);

	UFUNCTION(BlueprintCallable, Category="Battle|Skill")
	bool CastSkill();

	virtual TWeakPtr<FMWTargetSelector> GetTargetSelector() const override;
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

	TSharedPtr<FMWTargetSelector> TargetSelector;
#pragma endregion
	
#pragma region Camera
	TSharedPtr<FMWCameraComponent> CameraComponent;
#pragma endregion

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
