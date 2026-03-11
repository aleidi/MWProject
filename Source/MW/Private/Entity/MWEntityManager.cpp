#include "Entity/MWEntityManager.h"

#include "Component/Battle/MWBattleUnitComponent.h"
#include "Data/MWDataTableManager.h"
#include "Define/MWDefineDataTable.h"
#include "Entity/Character/MWCharacterEntity.h"
#include "Entity/MWEntity.h"
#include "MWLogChannels.h"
#include "System/MWGameInstanceSubsystem.h"

UMWEntityManager* UMWEntityManager::Get(const UObject* WorldContext)
{
	if (UMWGameInstanceSubsystem* Subsystem = UMWGameInstanceSubsystem::Get(WorldContext))
	{
		return Subsystem->GetEntityManager();
	}

	return nullptr;
}

UMWEntity* UMWEntityManager::FindEntity(const FObjectId EntityId)
{
	return nullptr;
}

UMWCharacterEntity* UMWEntityManager::CreateCharacterEntity(const FVector& Location, const FRotator& Rotation, const FMWCharacterSpawnResourceData* ResData)
{
	FObjectId newId = AllocateEntityId();

	return CreateCharacterEntity(newId, Location, Rotation, ResData);
}

UMWCharacterEntity* UMWEntityManager::CreateCharacterEntity(FObjectId EntityId, const FVector& Location, const FRotator& Rotation, const FMWCharacterSpawnResourceData* ResData)
{
	UMWCharacterEntity* characterEntity = NewObject<UMWCharacterEntity>(this);

	characterEntity->Initialize();
	characterEntity->SetSpawnData(EntityId, Location, Rotation);

	//set character data
	if (ResData)
	{
		characterEntity->SetCharacterData(ResData);
	}

	CharacterMap.Emplace(EntityId, characterEntity);

	AActor* charActor = characterEntity->LoadActor(true);
	charActor->FinishSpawning(FTransform(Rotation, Location));

	return characterEntity;
}

UMWCharacterEntity* UMWEntityManager::FindCharacter(const FObjectId& EntityId)
{
	UMWCharacterEntity** found = CharacterMap.Find(EntityId);
	return found ? *found : nullptr;
}

void UMWEntityManager::DespawnCharacter(const FObjectId& EntityId)
{
	RemoveCharacter(EntityId);
}

AMWCharacter* UMWEntityManager::SpawnCharacter(int32 CharacterId, const FVector& Location, const FRotator& Rotation)
{
	const FMWCharacterTemplateData* data = DATATABLEMANAGER->GetCharacterDataById(CharacterId);

	if (data)
	{
		FMWCharacterSpawnResourceData resData;
		resData.VisualAnimInst = data->CharacterAnimInst.LoadSynchronous();
		resData.VisualMesh = data->CharacterMesh.LoadSynchronous();
		resData.Height = data->Height;

		auto* charEntity = ENTITYMANAGER(this)->CreateCharacterEntity(Location, Rotation, &resData);
		check(charEntity);

		return charEntity->GetCharacterActor();
	}

	return nullptr;
}

AMWCharacter* UMWEntityManager::SpawnBattleUnitCharacter(int32 CharacterId1, int32 CharacterId2, int32 CharacterId3, const FVector& Location, const FRotator& Rotation)
{
	const FMWCharacterTemplateData* data1 = DATATABLEMANAGER->GetCharacterDataById(CharacterId1);
	const FMWCharacterTemplateData* data2 = DATATABLEMANAGER->GetCharacterDataById(CharacterId2);
	const FMWCharacterTemplateData* data3 = DATATABLEMANAGER->GetCharacterDataById(CharacterId3);

	if (data1 && data2 && data3)
	{
		FMWCharacterSpawnResourceData spawnData;
		spawnData.VisualAnimInst = data1->CharacterAnimInst.LoadSynchronous();
		spawnData.VisualMesh = data1->CharacterMesh.LoadSynchronous();
		spawnData.CharacterClass = AMWCharacter::StaticClass();

		UMWCharacterEntity* newEntity = ENTITYMANAGER(this)->CreateCharacterEntity(Location, Rotation, &spawnData);
		check(newEntity);

		AMWCharacter* character = Cast<AMWCharacter>(newEntity->GetCharacterActor());
		check(character);

		if (UMWBattleUnitComponent* unitComponent = character->FindComponentByClass<UMWBattleUnitComponent>())
		{
			TArray<FMWBattleUnitCharacterData> charDataArray;
			// Characer1
			{
				FMWBattleUnitCharacterData charData;
				charData.ID = CharacterId1;
				charData.Type = EBattleUnitCharacterType::Entity;
				charData.Mesh = data1->CharacterMesh.LoadSynchronous();
				charData.AnimInst = data1->CharacterAnimInst.LoadSynchronous();
				charData.SkillTable = data1->BattleSkillTable.LoadSynchronous();

				charDataArray.Emplace(charData);
			}
			// Character2
			{
				FMWBattleUnitCharacterData charData;
				charData.ID = CharacterId2;
				charData.Type = EBattleUnitCharacterType::Possession;
				charData.Mesh = data2->CharacterMesh.LoadSynchronous();
				charData.AnimInst = data2->CharacterAnimInst.LoadSynchronous();
				charData.SkillTable = data2->BattleSkillTable.LoadSynchronous();

				charDataArray.Emplace(charData);
			}
			// Character3
			{
				FMWBattleUnitCharacterData charData;
				charData.ID = CharacterId3;
				charData.Type = EBattleUnitCharacterType::Possession;
				charData.Mesh = data3->CharacterMesh.LoadSynchronous();
				charData.AnimInst = data3->CharacterAnimInst.LoadSynchronous();
				charData.SkillTable = data3->BattleSkillTable.LoadSynchronous();

				charDataArray.Emplace(charData);
			}

			unitComponent->SetCharacterData(charDataArray);
		}

		return character;
	}

	return nullptr;
}

void UMWEntityManager::RemoveCharacter(const FObjectId& EntityId)
{
	if (UMWCharacterEntity* charEntity = FindCharacter(EntityId))
	{
		charEntity->Uninitialize();
		CharacterMap.Remove(EntityId);
	}
}

FObjectId UMWEntityManager::AllocateEntityId()
{
	FObjectId newId;

	if (RecycledEntityIds.Num() > 0)
	{
		newId = RecycledEntityIds.Pop();
	}
	else
	{
		if (NextEntityId == UINT64_MAX)
		{
			newId = FObjectId(0);

			MW_LOG_WARNING(TEXT("Entity ID has reached the maximum limit!"));

			return newId;
		}

		newId = FObjectId(NextEntityId++);
	}

	ActiveEntityIds.Emplace(newId);

	return newId;
}

void UMWEntityManager::ReleaseEntityId(const FObjectId& EntityId)
{
	if (ActiveEntityIds.Remove(EntityId) > 0)
	{
		RecycledEntityIds.Add(EntityId);
	}
}

bool UMWEntityManager::IsEntityIdValid(const FObjectId& EntityId) const
{
	return ActiveEntityIds.Contains(EntityId);
}
