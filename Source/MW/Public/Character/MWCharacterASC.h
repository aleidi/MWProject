#pragma once

// Include Header
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "MWCharacterASC.generated.h"

// Forward Declare

// Define

/*
 * @class AMWCharacterASC
 * 
 * @brief
 *
 * @note
 */
UCLASS()
class MW_API AMWCharacterASC : public ACharacter, 
							   public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	AMWCharacterASC(const FObjectInitializer& ObjectInitializer);

	#pragma region GAS
public:
	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(Blueprintpure, Category = "MWCharacter|GAS")
	UMWAbilitySystemComponent* GetMWAbilitySystemComponent() const;

protected:
	// The ability system component sub-object used by player characters.
	UPROPERTY(Category=Camera, VisibleAnywhere, BlueprintReadOnly)
	UMWAbilitySystemComponent* AbilitySystemComponent;
#pragma endregion 
};