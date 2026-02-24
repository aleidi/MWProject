#include "Component/Battle/MWBattleUnitComponent.h"
#include "Data/MWDataTableManager.h"
#include "Define/MWDefineBattle.h"
#include "Entity/MWEntityManager.h"
#include "Gameplay/Battle/BattleUnit/MWBattleUnitAvatar.h"
#include "Gameplay/Battle/BattleUnit/MWStatusEffect.h"
#include "GameplayAbility/MWAbilitySet.h"
#include "MWLogChannels.h"

UMWBattleUnitComponent::UMWBattleUnitComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	EntityCharacterIdx = 0;
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
		EntityCharacterIdx = 0;

		return;
	}

	int32 increment = bNext ? 1 : -1;

	int32 newIdx = (EntityCharacterIdx + increment + CharacterData.Num()) % CharacterData.Num();

	const auto& data = CharacterData[newIdx];

	// Change character's appearance.
	// アバターの外観を変更する.
	auto* character = GetPawn<AMWCharacter>();
	if (character)
	{
		if (auto* mesh = character->GetVisualMesh())
		{
			mesh->SetSkeletalMesh(data.Mesh);
			mesh->SetAnimInstanceClass(data.AnimInst);
		}
	}

	// Update character type
	// キャラクタータイプを更新
	CharacterData[EntityCharacterIdx].Type = EBattleUnitCharacterType::Possession;
	CharacterData[newIdx].Type = EBattleUnitCharacterType::Entity;

	EntityCharacterIdx = newIdx;

	//TODO: Reset attribute
}

void UMWBattleUnitComponent::OnCharacterDataChanged()
{
	// Find entity character and set mesh and anim instance.
	// エンティティキャラクターを見つけて、メッシュとアニメーションインスタンスを設定する.
	int32 entityIdx = FindEntityCharacterId();
	if (entityIdx != INDEX_NONE)
	{
		EntityCharacterIdx = entityIdx;
		const auto& data = CharacterData[entityIdx];
		auto* character = GetPawn<AMWCharacter>();
		if (character)
		{
			if (auto* mesh = character->GetVisualMesh())
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
	return CharacterData.IsValidIndex(EntityCharacterIdx) ? CharacterData[EntityCharacterIdx].SkillTable : nullptr;
}

void UMWBattleUnitComponent::SetCombatState(bool bInCombat)
{
	// If the combat state is the same, do nothing.
	// 今の戦闘状態と一致した場合、何もしない.
	if(bIsInCombat == bInCombat)
	{
		return;
	}

	bIsInCombat = bInCombat;

	if (AMWCharacter* character = GetPawn<AMWCharacter>())
	{
		if (UMWAbilitySystemComponent* asc = character->GetMWAbilitySystemComponent())
		{
			if (bInCombat)
			{
				asc->AddLooseGameplayTag(MWGameplayTags::GP_Character_CombatState);

				OnCombatBegin();
			}
			else
			{
				asc->RemoveLooseGameplayTag(MWGameplayTags::GP_Character_CombatState);

				OnCombatEnd();
			}
		}
	}
}

bool UMWBattleUnitComponent::GetCombatState() const
{
	return bIsInCombat;
}

void UMWBattleUnitComponent::OnCombatBegin()
{
	// Grant combat abilities when entering combat
	GrandCombatAbilities();
}

void UMWBattleUnitComponent::OnCombatEnd()
{
	// Remove combat abilities when exiting combat
	RemoveCombatAbilities();
}

void UMWBattleUnitComponent::GrandCombatAbilities()
{
	AMWCharacter* owner = GetPawn<AMWCharacter>();
	
	if (!owner)
	{
		UE_LOG(LogMWBattle, Warning, TEXT("UMWBattleUnitComponent::GrandCombatAbilities: Owner is null."));
		return;
	}

	UMWAbilitySystemComponent* asc = owner->GetMWAbilitySystemComponent();
	if (!asc)
	{
		UE_LOG(LogMWBattle, Warning, TEXT("UMWBattleUnitComponent::GrandCombatAbilities: AbilitySystemComponent is null."));
		return;
	}

	if (!AbilitySet)
	{
		UE_LOG(LogMWBattle, Warning, TEXT("UMWBattleUnitComponent::GrandCombatAbilities: AbilitySet is null."));
		return;
	}

	// Initialize the granted handles if not already done
	if (!AbilityGranetedHandles.IsValid())
	{
		AbilityGranetedHandles = MakeShared<FMWAbilitySet_GrantedHandles>();
	}

	// Grant abilities and store the handles
	AbilitySet->GiveToAbilitySystem(asc, AbilityGranetedHandles.Get(), this);
}

void UMWBattleUnitComponent::RemoveCombatAbilities()
{
	AMWCharacter* owner = GetPawn<AMWCharacter>();
	
	if (!owner)
	{
		UE_LOG(LogMWBattle, Warning, TEXT("UMWBattleUnitComponent::RemoveCombatAbilities: Owner is null."));
		return;
	}

	UMWAbilitySystemComponent* asc = owner->GetMWAbilitySystemComponent();
	if (!asc)
	{
		UE_LOG(LogMWBattle, Warning, TEXT("UMWBattleUnitComponent::RemoveCombatAbilities: AbilitySystemComponent is null."));
		return;
	}

	// Check if we have granted handles to remove
	if (!AbilityGranetedHandles.IsValid() || !AbilitySet)
	{
		UE_LOG(LogMWBattle, Log, TEXT("UMWBattleUnitComponent::RemoveCombatAbilities: No abilities to remove."));
		return;
	}

	// Remove all granted abilities using the cached handles
	AbilityGranetedHandles->RemoveFromAbilitySystem(asc);
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
}
