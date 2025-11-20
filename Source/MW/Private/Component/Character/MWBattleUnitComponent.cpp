#include "Component/Character/MWBattleUnitComponent.h"
#include "Data/MWDataTableManager.h"
#include "Define/MWDefineBattle.h"
#include "Entity/MWEntityManager.h"
#include "Gameplay/Battle/BattleUnit/MWStatusEffect.h"
#include "Gameplay/Battle/BattleUnit/MWBattleUnitAvatar.h"
#include "Gameplay/Battle/BattleUnit/MWAttributeManager.h"

UMWBattleUnitComponent::UMWBattleUnitComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AvatarCharacterIdx = 0;
}

void UMWBattleUnitComponent::SetCharacterData(const TArray<FMWBattleUnitCharacterData>& InData)
{
	CharacterData = InData;
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

	// Reset attribute
}

const UMWCharacterBattleSkillDataAsset* UMWBattleUnitComponent::GetSkillData()
{
	return CharacterData.IsValidIndex(AvatarCharacterIdx) ? CharacterData[AvatarCharacterIdx].SkillTable : nullptr;
}
