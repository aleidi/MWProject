#include "Character/MWSkillComponent.h"

#include "Data/Skill/MWSkillAsset.h"
#include "Data/Skill/MWSkillDataManager.h"
#include "GameplayAbility/Ability/Skill/MWSkillCastPayload.h"
#include "GameplayAbility/MWAbilitySystemComponent.h"
#include "Gameplay/MWGameplayTags.h"
#include "MWLogChannels.h"
#include "System/MWConsoleVars.h"
#include "Util/UEDebugUtils.h"

#define Max_SKILL_SLOT_COUNT 11

UMWSkillComponent::UMWSkillComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	EquippedSkillSlots.Reserve(Max_SKILL_SLOT_COUNT);
	SkillCastEventTag = MWGameplayTags::Ability_Skill_Cast;
}

bool UMWSkillComponent::LearnSkill(int32 SkillId)
{
	if (SkillId == INDEX_NONE)
	{
		return false;
	}

	LearnedSkillIds.Add(SkillId);

	return true;
}

bool UMWSkillComponent::ForgetSkill(int32 SkillId, bool bUnequipIfEquipped)
{
	if (!LearnedSkillIds.Contains(SkillId))
	{
		return false;
	}

	LearnedSkillIds.Remove(SkillId);

	if (bUnequipIfEquipped)
	{
		for (auto& pair : EquippedSkillSlots)
		{
			if (pair.Value.EquippedSkillId == SkillId)
			{
				pair.Value.EquippedSkillId = INDEX_NONE;
				pair.Value.Animation = nullptr;
			}
		}
	}

	return true;
}

bool UMWSkillComponent::HasLearnedSkill(int32 SkillId) const
{
	return LearnedSkillIds.Contains(SkillId);
}

bool UMWSkillComponent::EquipSkillToSlot(int32 SkillId, int32 SlotIndex)
{
	if (!IsSlotValid(SlotIndex))
	{
		UE_LOG(LogMW, Warning, TEXT("EquipSkillToSlot: Invalid slot index %d"), SlotIndex);
		return false;
	}

	if (!HasLearnedSkill(SkillId))
	{
		UE_LOG(LogMW, Warning, TEXT("EquipSkillToSlot: Skill %d is not learned."), SkillId);
		return false;
	}

	FMWEquippedSkillSlot& slot = EquippedSkillSlots.FindOrAdd(SlotIndex);
	slot.EquippedSkillId = SkillId;

	const UMWSkillAsset* skillAsset = GET_SKILLDATAMGR(this)->GetLoadedSkillAsset(SkillId);
	slot.Animation = skillAsset && skillAsset->Animation.IsValid() ? skillAsset->Animation.Get() : nullptr;

	return true;
}

bool UMWSkillComponent::UnequipSlot(int32 SlotIndex)
{
	if (!IsSlotValid(SlotIndex))
	{
		UE_LOG(LogMW, Warning, TEXT("UnequipSlot: Invalid slot index %d"), SlotIndex);
		return false;
	}

	if (FMWEquippedSkillSlot* slot = EquippedSkillSlots.Find(SlotIndex))
	{
		slot->EquippedSkillId = INDEX_NONE;
		slot->Animation = nullptr;
	}

	return true;
}

void UMWSkillComponent::UnequipAllSlots()
{
	for (auto& pair : EquippedSkillSlots)
	{
		pair.Value.EquippedSkillId = INDEX_NONE;
		pair.Value.Animation = nullptr;
	}
}

int32 UMWSkillComponent::GetEquippedSkillId(int32 SlotIndex) const
{
	if (const FMWEquippedSkillSlot* slot = EquippedSkillSlots.Find(SlotIndex))
	{
		return slot->EquippedSkillId;
	}

	return INDEX_NONE;
}

UAnimMontage* UMWSkillComponent::GetEquippedSkillAnimation(int32 SlotIndex) const
{
	if (const FMWEquippedSkillSlot* slot = EquippedSkillSlots.Find(SlotIndex))
	{
		return slot->Animation;
	}

	return nullptr;
}

UAnimMontage* UMWSkillComponent::GetSkillAnimationByInputTag(const FGameplayTag& InputTag) const
{
	const int32 slotIndex = ResolveSkillSlotFromInputTag(InputTag);

	return IsSlotValid(slotIndex) ? GetEquippedSkillAnimation(slotIndex) : nullptr;
}

bool UMWSkillComponent::TryBuildCastCommandFromInputTag(const FGameplayTag& InputTag, FMWSkillCastCommand& OutCommand) const
{
	const int32 slotIndex = ResolveSkillSlotFromInputTag(InputTag);

	if (!IsSlotValid(slotIndex))
	{
		return false;
	}

	const int32 skillId = GetEquippedSkillId(slotIndex);

	if (skillId == INDEX_NONE)
	{
		return false;
	}

	OutCommand = FMWSkillCastCommand();
	OutCommand.Source = EMWSkillCastSource::PlayerInput;
	OutCommand.InputTag = InputTag;
	OutCommand.SkillSlot = slotIndex;
	OutCommand.SkillId = skillId;
	OutCommand.OverrideSection = ResolveMontageSectionFromInputTag(InputTag);
	OutCommand.Variant = OutCommand.OverrideSection != NAME_None ? EMWSkillCastVariant::Charge : EMWSkillCastVariant::Normal;

	return true;
}

bool UMWSkillComponent::TryResolveMontageData(const FMWSkillCastCommand& InCommand, UAnimMontage*& OutMontage, FName& OutSection) const
{
	OutMontage = nullptr;
	OutSection = NAME_None;

	int32 skillId = InCommand.SkillId;
	// スキルIDの取得方法は入力元の種別に応じて切り替える。
	// プレイヤー入力の場合はスロットインデックスからスキルIDを取得する。
	// If the source is from AI, we should get the skill id from the skill id in the command.
	if (skillId == INDEX_NONE && IsSlotValid(InCommand.SkillSlot))
	{
		skillId = GetEquippedSkillId(InCommand.SkillSlot);
	}

	if (skillId == INDEX_NONE)
	{
		return false;
	}

	if (!HasLearnedSkill(skillId))
	{
		return false;
	}

	const UMWSkillDataManager* skillDataMgr = GET_SKILLDATAMGR(this);
	const UMWSkillAsset* skillAsset = skillDataMgr ? skillDataMgr->GetLoadedSkillAsset(skillId) : nullptr;

	if (skillAsset)
	{
		OutMontage = skillAsset->Animation.IsValid() ? skillAsset->Animation.Get() : nullptr;
	}

	if (!OutMontage && IsSlotValid(InCommand.SkillSlot))
	{
		OutMontage = GetEquippedSkillAnimation(InCommand.SkillSlot);
	}

	if (!OutMontage)
	{
		return false;
	}

	OutSection = InCommand.OverrideSection;
	if (OutSection == NAME_None)
	{
		if (InCommand.Variant == EMWSkillCastVariant::Charge)
		{
			OutSection = TEXT("Charge");
		}
	}

	return true;
}

bool UMWSkillComponent::RequestCastByInputTag(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
	{
		return false;
	}

	UMWAbilitySystemComponent* asc = GetMWAbilitySystemComponent();
	if (!asc || !SkillCastEventTag.IsValid())
	{
		return false;
	}

	FMWSkillCastCommand command;
	if (!TryBuildCastCommandFromInputTag(InputTag, command))
	{
		return false;
	}

	UMWSkillCastPayload* payload = NewObject<UMWSkillCastPayload>(this);
	payload->Command = command;

	FGameplayEventData eventData;
	eventData.OptionalObject = payload;
	eventData.InstigatorTags.AddTag(InputTag);

	return asc->HandleGameplayEvent(SkillCastEventTag, &eventData) > 0;
}

bool UMWSkillComponent::RequestCastBySlot(int32 SlotIndex, FName MontageSection)
{
	if (!IsSlotValid(SlotIndex))
	{
		return false;
	}

	const int32 skillId = GetEquippedSkillId(SlotIndex);
	if (skillId == INDEX_NONE)
	{
		return false;
	}

	UMWAbilitySystemComponent* asc = GetMWAbilitySystemComponent();
	if (!asc || !SkillCastEventTag.IsValid())
	{
		return false;
	}

	FMWSkillCastCommand command;
	command.Source = EMWSkillCastSource::PlayerInput;
	command.SkillSlot = SlotIndex;
	command.SkillId = skillId;
	command.OverrideSection = MontageSection;
	command.Variant = MontageSection != NAME_None ? EMWSkillCastVariant::Charge : EMWSkillCastVariant::Normal;

	UMWSkillCastPayload* payload = NewObject<UMWSkillCastPayload>(this);
	payload->Command = command;

	FGameplayEventData eventData;
	eventData.OptionalObject = payload;

	return asc->HandleGameplayEvent(SkillCastEventTag, &eventData) > 0;
}

bool UMWSkillComponent::RequestCastBySkillId(int32 SkillId, EMWSkillCastVariant Variant, FName MontageSection)
{
	if (SkillId == INDEX_NONE || !HasLearnedSkill(SkillId))
	{
		return false;
	}

	UMWAbilitySystemComponent* asc = GetMWAbilitySystemComponent();
	if (!asc || !SkillCastEventTag.IsValid())
	{
		return false;
	}

	FMWSkillCastCommand command;
	command.Source = EMWSkillCastSource::AI;
	command.SkillId = SkillId;
	command.SkillSlot = INDEX_NONE;
	command.Variant = Variant;
	command.OverrideSection = MontageSection;

	UMWSkillCastPayload* payload = NewObject<UMWSkillCastPayload>(this);
	payload->Command = command;

	FGameplayEventData eventData;
	eventData.OptionalObject = payload;

	return asc->HandleGameplayEvent(SkillCastEventTag, &eventData) > 0;
}

void UMWSkillComponent::BeginPlay()
{
	Super::BeginPlay();

	//InitializeEquippedSkillSlots();

	SetComponentTickEnabled(true);
}

void UMWSkillComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnequipAllSlots();

	Super::EndPlay(EndPlayReason);
}

void UMWSkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//// ランタイム状態はサーバーを正とする。
	//const AActor* ownerActor = GetOwner();
	//if (!ownerActor || !ownerActor->HasAuthority())
	//{
	//	return;
	//}

	UpdateSkillUseRecovery(DeltaTime);
}

void UMWSkillComponent::InitializeEquippedSkillSlots()
{
	EquippedSkillSlots.Reset();

	for (int32 i = 0; i < Max_SKILL_SLOT_COUNT; ++i)
	{
		FMWEquippedSkillSlot& slot = EquippedSkillSlots.FindOrAdd(i);
		slot.EquippedSkillId = INDEX_NONE;
		slot.Animation = nullptr;
	}
}

bool UMWSkillComponent::IsSlotValid(int32 SlotIndex) const
{
	return SlotIndex >= 0 && SlotIndex < Max_SKILL_SLOT_COUNT;
}

int32 UMWSkillComponent::ResolveSkillSlotFromInputTag(const FGameplayTag& InputTag) const
{
	if (InputTag.MatchesTagExact(MWGameplayTags::IATag_TPBattle_CharacterSkillSlot1) || InputTag.MatchesTagExact(MWGameplayTags::IATag_TPBattle_CharacterSkillSlot1_Charge)) return 0;
	if (InputTag.MatchesTagExact(MWGameplayTags::IATag_TPBattle_CharacterSkillSlot2) || InputTag.MatchesTagExact(MWGameplayTags::IATag_TPBattle_CharacterSkillSlot2_Charge)) return 1;
	if (InputTag.MatchesTagExact(MWGameplayTags::IATag_TPBattle_CharacterSkillSlot3) || InputTag.MatchesTagExact(MWGameplayTags::IATag_TPBattle_CharacterSkillSlot3_Charge)) return 2;
	if (InputTag.MatchesTagExact(MWGameplayTags::IATag_TPBattle_CharacterSkillSlot4)) return 3;

	if (InputTag.MatchesTagExact(MWGameplayTags::IATag_TPBattle_CharacterExtraSkillSlot1)) return 4;
	if (InputTag.MatchesTagExact(MWGameplayTags::IATag_TPBattle_CharacterExtraSkillSlot2)) return 5;
	if (InputTag.MatchesTagExact(MWGameplayTags::IATag_TPBattle_CharacterExtraSkillSlot3)) return 6;

	if (InputTag.MatchesTagExact(MWGameplayTags::IATag_TPBattle_PartnerSkillSlot1)) return 7;
	if (InputTag.MatchesTagExact(MWGameplayTags::IATag_TPBattle_PartnerSkillSlot2)) return 8;
	if (InputTag.MatchesTagExact(MWGameplayTags::IATag_TPBattle_PartnerSkillSlot3)) return 9;
	if (InputTag.MatchesTagExact(MWGameplayTags::IATag_TPBattle_PartnerSkillSlot4)) return 10;

	return INDEX_NONE;
}

FName UMWSkillComponent::ResolveMontageSectionFromInputTag(const FGameplayTag& InputTag) const
{
	if (InputTag.MatchesTagExact(MWGameplayTags::IATag_TPBattle_CharacterSkillSlot1_Charge)
		|| InputTag.MatchesTagExact(MWGameplayTags::IATag_TPBattle_CharacterSkillSlot2_Charge)
		|| InputTag.MatchesTagExact(MWGameplayTags::IATag_TPBattle_CharacterSkillSlot3_Charge))
	{
		return TEXT("Charge");
	}

	return NAME_None;
}

UMWAbilitySystemComponent* UMWSkillComponent::GetMWAbilitySystemComponent() const
{
	const AActor* ownerActor = GetOwner();
	return ownerActor ? ownerActor->FindComponentByClass<UMWAbilitySystemComponent>() : nullptr;
}

bool UMWSkillComponent::TryGetRuntimeSkillState(int32 SkillId, FMWRuntimeSkillState& OutState) const
{
	const FMWRuntimeSkillState* runtimeState = SkillRuntimeStates.Find(SkillId);
	if (!runtimeState)
	{
		return false;
	}

	OutState = *runtimeState;

	return true;
}

bool UMWSkillComponent::ConsumeSkillUse(int32 SkillId, int32 ConsumeAmount)
{
	if (!TryInitializeRuntimeSkillState(SkillId))
	{
		return false;
	}

	FMWRuntimeSkillState* runtimeState = SkillRuntimeStates.Find(SkillId);
	if (!runtimeState)
	{
		return false;
	}

	const int32 safeConsumeAmount = FMath::Max(1, ConsumeAmount);
	if (runtimeState->CurrentUses < safeConsumeAmount)
	{
		return false;
	}

	runtimeState->CurrentUses -= safeConsumeAmount;

	if (const UWorld* world = GetWorld())
	{
		runtimeState->LastConsumeWorldTime = world->GetTimeSeconds();
	}
	else
	{
		runtimeState->LastConsumeWorldTime = 0.0f;
	}

	return true;
}

bool UMWSkillComponent::RecoverSkillUse(int32 SkillId, int32 RecoverAmount)
{
	if (!TryInitializeRuntimeSkillState(SkillId))
	{
		return false;
	}

	FMWRuntimeSkillState* runtimeState = SkillRuntimeStates.Find(SkillId);
	if (!runtimeState)
	{
		return false;
	}

	const int32 safeRecoverAmount = FMath::Max(1, RecoverAmount);
	const int32 oldUses = runtimeState->CurrentUses;
	runtimeState->CurrentUses = FMath::Min(runtimeState->CurrentUses + safeRecoverAmount, runtimeState->MaxUses);

	return runtimeState->CurrentUses != oldUses;
}

bool UMWSkillComponent::CanConsumeSkillUse(int32 SkillId, int32 RequiredAmount /*= 1*/)
{
	if (!TryInitializeRuntimeSkillState(SkillId))
	{
		return false;
	}

	const FMWRuntimeSkillState* runtimeState = SkillRuntimeStates.Find(SkillId);
	if (!runtimeState)
	{
		return false;
	}

	return runtimeState->CurrentUses >= FMath::Max(1, RequiredAmount);
}

bool UMWSkillComponent::TryInitializeRuntimeSkillState(int32 SkillId)
{
	if (SkillId == INDEX_NONE)
	{
		return false;
	}

	if (SkillRuntimeStates.Contains(SkillId))
	{
		return true;
	}

	const UMWSkillDataManager* skillDataMgr = GET_SKILLDATAMGR(this);
	const FMWSkillTable* skillRow = skillDataMgr ? skillDataMgr->FindSkillRow(SkillId) : nullptr;
	if (!skillRow)
	{
		return false;
	}

	const FMWSkillStockConfig& stockConfig = skillRow->StockConfig;

	FMWRuntimeSkillState runtimeState;
	runtimeState.SkillId = SkillId;
	runtimeState.MaxUses = FMath::Max(1, stockConfig.MaxUses);
	runtimeState.CurrentUses = runtimeState.MaxUses;
	runtimeState.RecoverAmount = FMath::Max(0.0f, stockConfig.RecoverAmount);
	runtimeState.RecoverPointThreshold = FMath::Max(1.0f, stockConfig.RecoverPointThreshold);
	runtimeState.RecoverPointAccumulated = 0.0f;
	runtimeState.RecoverDelayAfterConsume = FMath::Max(0.0f, stockConfig.RecoverDelayAfterConsume);
	runtimeState.LastConsumeWorldTime = 0.0f;

	SkillRuntimeStates.Add(SkillId, runtimeState);

	return true;
}

void UMWSkillComponent::UpdateSkillUseRecovery(float DeltaTime)
{
	if (DeltaTime <= 0.0f)
	{
		return;
	}

	const UWorld* world = GetWorld();
	const float nowTime = world ? world->GetTimeSeconds() : 0.0f;

	for (auto& pair : SkillRuntimeStates)
	{
		FMWRuntimeSkillState& runtimeState = pair.Value;

		if (runtimeState.CurrentUses >= runtimeState.MaxUses)
		{
			runtimeState.RecoverPointAccumulated = 0.0f;

			continue;
		}

		if (runtimeState.RecoverAmount <= 0.0f || runtimeState.RecoverPointThreshold <= 0.0f)
		{
			continue;
		}

		if ((nowTime - runtimeState.LastConsumeWorldTime) < runtimeState.RecoverDelayAfterConsume)
		{
			continue;
		}

		runtimeState.RecoverPointAccumulated += runtimeState.RecoverAmount * DeltaTime;

		if (runtimeState.RecoverPointAccumulated >= runtimeState.RecoverPointThreshold)
		{
			runtimeState.CurrentUses = FMath::Min(runtimeState.CurrentUses + 1, runtimeState.MaxUses);

			runtimeState.RecoverPointAccumulated = 0.0f;

			UE_SCREEN_PRINT_CVAR(MWConsoleVars::CVarShowSkillDebug, 0.f, FColor::Yellow, TEXT("Skill %d recovered one use. CurrentUses: %d"), runtimeState.SkillId, runtimeState.CurrentUses);
		}
	}
}

bool UMWSkillComponent::TryGetChargeParamsByInputTag( const FGameplayTag& InInputTag, float& OutMaxChargeValue, float& OutChargeRate, float& OutDischargeRate, float& OutChargeStartDelay) const
{
	const int32 slotIndex = ResolveSkillSlotFromInputTag(InInputTag);
	if (!IsSlotValid(slotIndex))
	{
		return false;
	}

	const int32 skillId = GetEquippedSkillId(slotIndex);
	if (skillId == INDEX_NONE)
	{
		return false;
	}

	const UMWSkillDataManager* skillDataMgr = GET_SKILLDATAMGR(this);
	const FMWSkillTable* skillRow = skillDataMgr ? skillDataMgr->FindSkillRow(skillId) : nullptr;
	if (!skillRow)
	{
		return false;
	}

	OutMaxChargeValue = skillRow->ChargeConfig.MaxValue;
	OutChargeRate = skillRow->ChargeConfig.GetChargeRate();
	OutDischargeRate = skillRow->ChargeConfig.DischargeRate;
	OutChargeStartDelay = skillRow->ChargeConfig.StartDelay;

	return true;
}
