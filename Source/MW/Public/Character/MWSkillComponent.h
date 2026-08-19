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

USTRUCT(BlueprintType)
struct FMWRuntimeSkillState
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MW|Skill|Runtime")
	int32 SkillId = INDEX_NONE;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MW|Skill|Runtime")
	int32 CurrentUses = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MW|Skill|Runtime")
	int32 MaxUses = 0;

	/** 1秒あたりに獲得する回復ポイント。 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MW|Skill|Runtime")
	float RecoverAmount = 100.0f;

	/** 1回分を回復するために必要なポイント。 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MW|Skill|Runtime")
	float RecoverPointThreshold = 100.0f;

	/** 実行時に蓄積された回復ポイント。 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MW|Skill|Runtime")
	float RecoverPointAccumulated = 0.0f;

	/** 消費後、回復を開始するまでの遅延時間。 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MW|Skill|Runtime")
	float RecoverDelayAfterConsume = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MW|Skill|Runtime")
	float LastConsumeWorldTime = 0.0f;
};

UCLASS(ClassGroup = (MW), meta = (BlueprintSpawnableComponent))
class MW_API UMWSkillComponent : public UMWPawnComponent
{
	GENERATED_BODY()

#pragma region LearnSkill/Loudout
public:
	UMWSkillComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// ===== スキルブック（習得済み） =====
	UFUNCTION(BlueprintCallable, Category = "MW|Skill|Book")
	bool LearnSkill(int32 SkillId);

	UFUNCTION(BlueprintCallable, Category = "MW|Skill|Book")
	bool ForgetSkill(int32 SkillId, bool bUnequipIfEquipped = true);

	UFUNCTION(BlueprintPure, Category = "MW|Skill|Book")
	bool HasLearnedSkill(int32 SkillId) const;

	// ===== ロードアウト（装備済み） =====
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

	// ===== コマンド =====
	bool TryBuildCastCommandFromInputTag(const FGameplayTag& InputTag, FMWSkillCastCommand& OutCommand) const;
	bool TryResolveMontageData(const FMWSkillCastCommand& InCommand, UAnimMontage*& OutMontage, FName& OutSection) const;

	// ===== キャストエントリ =====
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
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

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
#pragma endregion

#pragma region Runtime Skill State
public:
	UFUNCTION(BlueprintCallable, Category = "MW|Skill|Runtime")
	bool TryGetRuntimeSkillState(int32 SkillId, FMWRuntimeSkillState& OutState) const;

	UFUNCTION(BlueprintCallable, Category = "MW|Skill|Runtime")
	bool ConsumeSkillUse(int32 SkillId, int32 ConsumeAmount = 1);

	UFUNCTION(BlueprintCallable, Category = "MW|Skill|Runtime")
	bool RecoverSkillUse(int32 SkillId, int32 RecoverAmount = 1);

	UFUNCTION(BlueprintCallable, Category = "MW|Skill|Runtime")
	bool CanConsumeSkillUse(int32 SkillId, int32 RequiredAmount = 1);

private:
	bool TryInitializeRuntimeSkillState(int32 SkillId);

	void UpdateSkillUseRecovery(float DeltaTime);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MW|Skill|Runtime", meta = (AllowPrivateAccess = "true"))
	TMap<int32, FMWRuntimeSkillState> SkillRuntimeStates;
#pragma endregion

};
