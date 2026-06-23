#include "Character/MWSkillComponent.h"

#include "Data/Skill/MWSkillAsset.h"
#include "Data/Skill/MWSkillDataManager.h"
#include "Gameplay/MWGameplayTags.h"
#include "MWLogChannels.h"

#define Max_SKILL_SLOT_COUNT 11

UMWSkillComponent::UMWSkillComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;

	EquippedSkillSlots.Reserve(Max_SKILL_SLOT_COUNT);
}

bool UMWSkillComponent::EquipSkillToSlot(int32 SkillId, int32 SlotIndex)
{
	if (!IsSlotValid(SlotIndex))
	{
		UE_LOG(LogMW, Warning, TEXT("EquipSkillToSlot: Invalid slot index %d"), SlotIndex);

		return false;
	}

	FMWEquippedSkillSlot& Slot = EquippedSkillSlots.FindOrAdd(SlotIndex);
	Slot.EquippedSkillId = SkillId;

	const UMWSkillAsset* SkillAsset = GET_SKILLDATAMGR(this)->GetLoadedSkillAsset(SkillId);
	if (SkillAsset)
	{
		Slot.Animation = SkillAsset->Animation.IsValid() ? SkillAsset->Animation.Get() : nullptr;
	}

	return true;
}

bool UMWSkillComponent::UnequipSlot(int32 SlotIndex)
{
	if(!IsSlotValid(SlotIndex))
	{
		UE_LOG(LogMW, Warning, TEXT("UnequipSlot: Invalid slot index %d"), SlotIndex);
	
		return false;
	}

	if(FMWEquippedSkillSlot* Slot = EquippedSkillSlots.Find(SlotIndex))
	{
		Slot->EquippedSkillId = INDEX_NONE;
		Slot->Animation = nullptr;
	}

	return true;
}

void UMWSkillComponent::UnequipAllSlots()
{
	for(auto& Pair : EquippedSkillSlots)
	{
		Pair.Value.EquippedSkillId = INDEX_NONE;
		Pair.Value.Animation = nullptr;
	}
}

UAnimMontage* UMWSkillComponent::GetSkillAnimation(int32 SlotIndex) const
{
	if (const FMWEquippedSkillSlot* Slot = EquippedSkillSlots.Find(SlotIndex))
	{
		return Slot->Animation;
	}

	return nullptr;
}

void UMWSkillComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UMWSkillComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnequipAllSlots();

	Super::EndPlay(EndPlayReason);
}

void UMWSkillComponent::InitializeEquippedSkillSlots()
{
	EquippedSkillSlots.Reset();

	for(int32 i = 0; i < Max_SKILL_SLOT_COUNT; ++i)
	{
		FMWEquippedSkillSlot& Slot = EquippedSkillSlots.FindOrAdd(i);
		Slot.EquippedSkillId = INDEX_NONE;
		Slot.Animation = nullptr;
	}
}

bool UMWSkillComponent::IsSlotValid(int32 SlotIndex) const
{
	return SlotIndex >= 0 && SlotIndex < Max_SKILL_SLOT_COUNT;
}

UAnimMontage* UMWSkillComponent::GetSkillAnimationByInputTag(const FGameplayTag& InputTag) const
{
	const int32 SlotIndex = ResolveSkillSlotFromInputTag(InputTag);
	if (!IsSlotValid(SlotIndex))
	{
		return nullptr;
	}

	return GetSkillAnimation(SlotIndex);
}

int32 UMWSkillComponent::ResolveSkillSlotFromInputTag(const FGameplayTag& InputTag) const
{
	// Character skill slots: 0 ~ 3
	if (InputTag.MatchesTagExact(MWGameplayTags::IATag_TPBattle_CharacterSkillSlot1)
		|| InputTag.MatchesTagExact(MWGameplayTags::IATag_TPBattle_CharacterSkillSlot1_Charge))
	{
		return 0;
	}

	if (InputTag.MatchesTagExact(MWGameplayTags::IATag_TPBattle_CharacterSkillSlot2)
		|| InputTag.MatchesTagExact(MWGameplayTags::IATag_TPBattle_CharacterSkillSlot2_Charge))
	{
		return 1;
	}

	if (InputTag.MatchesTagExact(MWGameplayTags::IATag_TPBattle_CharacterSkillSlot3)
		|| InputTag.MatchesTagExact(MWGameplayTags::IATag_TPBattle_CharacterSkillSlot3_Charge))
	{
		return 2;
	}

	if (InputTag.MatchesTagExact(MWGameplayTags::IATag_TPBattle_CharacterSkillSlot4))
	{
		return 3;
	}

	// Character extra skill slots: 4 ~ 6
	if (InputTag.MatchesTagExact(MWGameplayTags::IATag_TPBattle_CharacterExtraSkillSlot1))
	{
		return 4;
	}

	if (InputTag.MatchesTagExact(MWGameplayTags::IATag_TPBattle_CharacterExtraSkillSlot2))
	{
		return 5;
	}

	if (InputTag.MatchesTagExact(MWGameplayTags::IATag_TPBattle_CharacterExtraSkillSlot3))
	{
		return 6;
	}

	// Partner skill slots: 7 ~ 10
	if (InputTag.MatchesTagExact(MWGameplayTags::IATag_TPBattle_PartnerSkillSlot1))
	{
		return 7;
	}

	if (InputTag.MatchesTagExact(MWGameplayTags::IATag_TPBattle_PartnerSkillSlot2))
	{
		return 8;
	}

	if (InputTag.MatchesTagExact(MWGameplayTags::IATag_TPBattle_PartnerSkillSlot3))
	{
		return 9;
	}

	if (InputTag.MatchesTagExact(MWGameplayTags::IATag_TPBattle_PartnerSkillSlot4))
	{
		return 10;
	}

	return INDEX_NONE;
}
