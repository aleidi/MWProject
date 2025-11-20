#pragma once

// Include Header
#include "UObject/NoExportTypes.h"
#include "Entity/MWObjctId.h"
#include "MWEntityManager.generated.h"

// Forward Declare
class UMWEntity;
class UMWCharacterEntity;
struct FMWCharacterSpawnResourceData;

// Define
#define ENTITYMANAGER(WorldContext) (UMWEntityManager::Get(WorldContext))

/*
 * @class UMWEntityManager
 * 
 * @brief
 *
 * @note
 */
UCLASS()
class MW_API UMWEntityManager : public UObject
{
	GENERATED_BODY()

public:
	void Initialize() {}
	void Deinitialize() {}

	UFUNCTION(BlueprintPure, Category = "Manager", meta = (WorldContext="WorldContext", DisplayName="GetCharacterManager"))
	static UMWEntityManager* Get(const UObject* WorldContext);

	UMWEntity* FindEntity(const FObjectId EntityId);

	template<typename _EntityType> requires TIsDerivedFrom<_EntityType, UMWEntity>::Value
		_EntityType* FindEntity(const FObjectId& EntityId)
	{
		return Cast<_EntityType>(FindEntity(EntityId));
	}

	//===============================================================
	// Character
	//===============================================================
public:
	UMWCharacterEntity* CreateCharacterEntity(FObjectId EntityId, const FVector& Location = FVector::ZeroVector, const FRotator& Rotation = FRotator::ZeroRotator, const FMWCharacterSpawnResourceData* ResData = nullptr);

	UMWCharacterEntity* CreateCharacterEntity(const FVector& Location = FVector::ZeroVector, const FRotator& Rotation = FRotator::ZeroRotator, const FMWCharacterSpawnResourceData* ResData = nullptr);

	UMWCharacterEntity* FindCharacter(const FObjectId& EntityId);

	void DespawnCharacter(const FObjectId& EntityId);

private:
	void RemoveCharacter(const FObjectId& EntityId);

private:
	UPROPERTY()
	TMap<FObjectId, UMWCharacterEntity*> CharacterMap;

//==============================================//
//            Entity ID Management              //
//==============================================//
private:
	// Allocate a new entity id, prefer to get from recycled pool.
	// 新しいエンティティIDを割り当て、優先的に再利用プールから取得する。
	FObjectId AllocateEntityId();

	// Release an entity id back to recycled pool.
	// エンティティIDを再利用プールに戻す。
	void ReleaseEntityId(const FObjectId& EntityId);

	// Check whether an entity id is valid (active).
	// エンティティIDが有効かどうかを確認する。
	bool IsEntityIdValid(const FObjectId& EntityId) const;

private:
	// Id starts from 1, 0 is reserve for invalid id.
	// IDは1から始まり、0は無効なIDとして使う。
	uint64 NextEntityId = 1;

	// Recycled Entity Ids
	// 再利用可能なエンティティID
	TArray<FObjectId> RecycledEntityIds;

	// Active Entity Ids
	// アクティブなエンティティID
	TSet<FObjectId> ActiveEntityIds;

};