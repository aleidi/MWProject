#pragma once

#include "GameplayAbility/Ability/Skill/MWSkillCastTypes.h"
#include "GameplayTagContainer.h"
#include "Pawn/MWPawnComponent.h"
#include "MWSkillComponent.generated.h"

class UAnimMontage;
class UMWSkillAsset;
class UMWAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FMWEquippedSkillSlot
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill")
	int32 EquippedSkillId = INDEX_NONE;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill")
	TObjectPtr<UAnimMontage> Animation = nullptr;
};

UCLASS(ClassGroup = (MW), meta = (BlueprintSpawnableComponent))
class MW_API UMWSkillComponent : public UMWPawnComponent
{
	GENERATED_BODY()

public:
	UMWSkillComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// ===== SkillBook（Learned） =====
	UFUNCTION(BlueprintCallable, Category = "MW|Skill|Book")
	bool LearnSkill(int32 SkillId);

	UFUNCTION(BlueprintCallable, Category = "MW|Skill|Book")
	bool ForgetSkill(int32 SkillId, bool bUnequipIfEquipped = true);

	UFUNCTION(BlueprintPure, Category = "MW|Skill|Book")
	bool HasLearnedSkill(int32 SkillId) const;

	// ===== Loadout（Equipped） =====
	UFUNCTION(BlueprintCallable, Category = "MW|Skill|Loadout")
	bool EquipSkillToSlot(int32 SkillId, int32 SlotIndex);

	UFUNCTION(BlueprintCallable, Category = "MW|Skill|Loadout")
	bool UnequipSlot(int32 SlotIndex);

	UFUNCTION(BlueprintCallable, Category = "MW|Skill|Loadout")
	void UnequipAllSlots();

	UFUNCTION(BlueprintPure, Category = "MW|Skill|Loadout")
	int32 GetEquippedSkillId(int32 SlotIndex) const;

	UAnimMontage* GetEquippedSkillAnimation(int32 SlotIndex) const;
	UAnimMontage* GetSkillAnimationByInputTag(const FGameplayTag& InputTag) const;

	// ===== Command =====
	bool TryBuildCastCommandFromInputTag(const FGameplayTag& InputTag, FMWSkillCastCommand& OutCommand) const;
	bool TryResolveMontageData(const FMWSkillCastCommand& InCommand, UAnimMontage*& OutMontage, FName& OutSection) const;

	// ===== Cast Entry =====
	UFUNCTION(BlueprintCallable, Category = "MW|Skill|Cast")
	bool RequestCastByInputTag(const FGameplayTag& InputTag);

	UFUNCTION(BlueprintCallable, Category = "MW|Skill|Cast")
	bool RequestCastBySlot(int32 SlotIndex, FName MontageSection = NAME_None);

	UFUNCTION(BlueprintCallable, Category = "MW|Skill|Cast")
	bool RequestCastBySkillId(int32 SkillId, EMWSkillCastVariant Variant = EMWSkillCastVariant::Normal, FName MontageSection = NAME_None);

public:
	bool TryGetChargeParamsByInputTag(const FGameplayTag& InInputTag, float& OutMaxChargeValue, float& OutChargeRate, float& OutDischargeRate, float& OutChargeStartDelay) const;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void InitializeEquippedSkillSlots();
	bool IsSlotValid(int32 SlotIndex) const;
	int32 ResolveSkillSlotFromInputTag(const FGameplayTag& InputTag) const;
	FName ResolveMontageSectionFromInputTag(const FGameplayTag& InputTag) const;

	UMWAbilitySystemComponent* GetMWAbilitySystemComponent() const;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MW|Skill|Cast", meta = (AllowPrivateAccess = "true"))
	FGameplayTag SkillCastEventTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MW|Skill", meta = (AllowPrivateAccess = "true"))
	TMap<int32, FMWEquippedSkillSlot> EquippedSkillSlots;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MW|Skill", meta = (AllowPrivateAccess = "true"))
	TSet<int32> LearnedSkillIds;
};
