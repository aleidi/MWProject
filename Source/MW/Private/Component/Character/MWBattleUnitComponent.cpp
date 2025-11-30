#include "Component/Character/MWBattleUnitComponent.h"
#include "Data/MWDataTableManager.h"
#include "Define/MWDefineBattle.h"
#include "Entity/MWEntityManager.h"
#include "Gameplay/Battle/BattleUnit/MWAttributeManager.h"
#include "Gameplay/Battle/BattleUnit/MWBattleUnitAvatar.h"
#include "Gameplay/Battle/BattleUnit/MWStatusEffect.h"
#include "GameplayAbility/MWAbilitySet.h"
#include "MWLogChannels.h"

UMWBattleUnitComponent::UMWBattleUnitComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AvatarCharacterIdx = 0;
}

void UMWBattleUnitComponent::SetCharacterData(const TArray<FMWBattleUnitCharacterData>& InData)
{
	CharacterData = InData;

	OnCharacterDataChanged();
}

void UMWBattleUnitComponent::ChangeNextAvatar(bool bNext)
{
	int32 charNum = CharacterData.Num();

	if (charNum == 0)
	{
		return;
	}

	// only one character, don't change.
	// キャラクターが一人だけの場合、変更しない.
	if (charNum == 1)
	{
		AvatarCharacterIdx = 0;

		return;
	}

	int32 increment = bNext ? 1 : -1;

	int32 newIdx = (AvatarCharacterIdx + increment + CharacterData.Num()) % CharacterData.Num();

	const auto& data = CharacterData[newIdx];

	// Change avatar's appearance.
	// アバターの外観を変更する.
	auto* avatar = GetPawn<AMWBattleUnitAvatar>();
	if (avatar)
	{
		if (auto* mesh = avatar->GetVisualMesh())
		{
			mesh->SetSkeletalMesh(data.Mesh);
			mesh->SetAnimInstanceClass(data.AnimInst);
		}
	}

	// Update character type
	// キャラクタータイプを更新
	CharacterData[AvatarCharacterIdx].Type = EBattleUnitCharacterType::Possession;
	CharacterData[newIdx].Type = EBattleUnitCharacterType::Entity;

	AvatarCharacterIdx = newIdx;

	//TODO: Reset attribute
}

void UMWBattleUnitComponent::OnCharacterDataChanged()
{
	// Find entity character and set mesh and anim instance.
	// エンティティキャラクターを見つけて、メッシュとアニメーションインスタンスを設定する.
	int32 entityIdx = FindEntityCharacterId();
	if (entityIdx != INDEX_NONE)
	{
		AvatarCharacterIdx = entityIdx;
		const auto& data = CharacterData[entityIdx];
		auto* avatar = GetPawn<AMWBattleUnitAvatar>();
		if (avatar)
		{
			if (auto* mesh = avatar->GetVisualMesh())
			{
				mesh->SetSkeletalMesh(data.Mesh);
				mesh->SetAnimInstanceClass(data.AnimInst);
			}
		}
	}

	//TODO: Reset attribute according to new character data.
}

const UMWCharacterBattleSkillDataAsset* UMWBattleUnitComponent::GetSkillData()
{
	return CharacterData.IsValidIndex(AvatarCharacterIdx) ? CharacterData[AvatarCharacterIdx].SkillTable : nullptr;
}

int32 UMWBattleUnitComponent::FindEntityCharacterId() const
{
	for(int32 i = 0; i < CharacterData.Num(); ++i)
	{
		if (CharacterData[i].Type == EBattleUnitCharacterType::Entity)
		{
			return i;
		}
	}

	UE_LOG(LogMWBattle, Warning, TEXT("UMWBattleUnitComponent::FindEntityCharacterId: No entity character found in CharacterData."));

	return INDEX_NONE;
}

void UMWBattleUnitComponent::BeginPlay()
{
	Super::BeginPlay();

	AMWBattleUnitAvatar* owner = GetPawn<AMWBattleUnitAvatar>();

	if (AbilitySet)
	{
		AbilitySet->GiveToAbilitySystem(owner->GetMWAbilitySystemComponent(), AbilityGranetedHandles.Get(), this);
	}
}
