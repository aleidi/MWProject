#pragma once

// Include Header
#include "GameplayAbilitySpecHandle.h"
#include "GameplayTagContainer.h"
#include "Pawn/MWPawnComponent.h"
#include "MWSkillComponent.generated.h"

// Forward Declare
class UMWSkillAsset;
class UMWAbilitySystemComponent;

// Define

USTRUCT(BlueprintType)
struct FMWEquippedSkillSlot
{
	GENERATED_BODY()

	/** Equipped skill id, INDEX_NONE means empty. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill")
	int32 EquippedSkillId = INDEX_NONE;

	/** Skill animation.*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill")
	TObjectPtr<UAnimMontage> Animation;
};

/*
 * @class UMWSkillComponent
 * 
 * @brief  Manage character's skills, including only equipped skills
 *
 * @note
 */
UCLASS(ClassGroup = (MW), meta = (BlueprintSpawnableComponent))
class MW_API UMWSkillComponent : public UMWPawnComponent
{
	GENERATED_BODY()

public:
	UMWSkillComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "MW|Skill|Loadout")
	bool EquipSkillToSlot(int32 SkillId, int32 SlotIndex);

	UFUNCTION(BlueprintCallable, Category = "MW|Skill|Loadout")
	bool UnequipSlot(int32 SlotIndex);

	UFUNCTION(BlueprintCallable, Category = "MW|Skill|Loadout")
	void UnequipAllSlots();

	UAnimMontage* GetSkillAnimation(int32 SlotIndex) const;
	UAnimMontage* GetSkillAnimationByInputTag(const FGameplayTag& InputTag) const;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void InitializeEquippedSkillSlots();
	bool IsSlotValid(int32 SlotIndex) const;
	int32 ResolveSkillSlotFromInputTag(const FGameplayTag& InputTag) const;

private:
	UPROPERTY(/*Transient, */EditDefaultsOnly, BlueprintReadOnly, Category = "MW|Skill", meta = (AllowPrivateAccess = "true"))
	TMap<int32, FMWEquippedSkillSlot> EquippedSkillSlots;
};