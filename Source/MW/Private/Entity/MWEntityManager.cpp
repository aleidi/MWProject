#include "Entity/MWEntityManager.h"
#include "Entity/MWEntity.h"
#include "System/MWGameInstanceSubsystem.h"
#include "Entity/MWCharacterEntity.h"

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

UMWCharacterEntity* UMWEntityManager::CreateCharacterEntity(const FVector& Location, const FRotator& Rotation)
{
	UMWCharacterEntity* charaEntity = NewObject<UMWCharacterEntity>(this);

	charaEntity->Initialize();

	FObjectId entityId = FObjectId::Generate();

	charaEntity->SetId(entityId);

	CharacterMap.Emplace(entityId, charaEntity);

	AActor* charActor = charaEntity->LoadActor(true);
	charActor->FinishSpawning(FTransform(Rotation, Location));

	return charaEntity;
}

UMWCharacterEntity* UMWEntityManager::SpawnCharacter(const FVector& Location, const FRotator& Rotation)
{
	UMWCharacterEntity* newCharEntity = CreateCharacterEntity(Location, Rotation);
	if (newCharEntity)
	{
		// TODO : something initialization for character
	}

	return newCharEntity;
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
