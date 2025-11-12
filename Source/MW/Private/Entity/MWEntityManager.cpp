#include "Entity/MWEntityManager.h"
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

UMWCharacterEntity* UMWEntityManager::CreateCharacterEntity(const FVector& Location, const FRotator& Rotation, const FMWCharacterResourceData* ResData)
{
	FObjectId newId = AllocateEntityId();

	return CreateCharacterEntity(newId, Location, Rotation, ResData);
}

UMWCharacterEntity* UMWEntityManager::CreateCharacterEntity(FObjectId EntityId, const FVector& Location, const FRotator& Rotation, const FMWCharacterResourceData* ResData)
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
