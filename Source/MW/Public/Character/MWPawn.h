#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AbilitySystemInterface.h"
#include "Gameplay/MWSelectableInterface.h"
#include "MWPawn.generated.h"

UCLASS()
class MW_API AMWPawn : public APawn, public IAbilitySystemInterface, public IMWSelectableInterface
{
	GENERATED_BODY()

public:
	AMWPawn(const FObjectInitializer& ObjectInitializer);
	
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
	UFUNCTION(Blueprintpure, Category = "MWPawn|GAS", meta = (DisplayName = "GetAbilitySystemComponent"))
	UAbilitySystemComponent* K2_GetAbilitySystemComponent() const;
	void InitStartupAbilities();

	// Only called on the Server. Calls before Server's AcknowledgePossession.
	virtual void PossessedBy(AController* NewController) override;

protected:
	//UPROPERTY(EditAnywhere, Category = "NWCharacter|GAS")
	//TObjectPtr<class UXXGameplayAbilitySet> StartupAbillities;
#pragma endregion 
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
