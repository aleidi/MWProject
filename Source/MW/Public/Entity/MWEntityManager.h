#pragma once

// Include Header
#include "UObject/NoExportTypes.h"
#include "Entity/MWObjctId.h"
#include "MWEntityManager.generated.h"

// Forward Declare
class UMWEntity;
class UMWCharacterEntity;

// Macro

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
	UMWCharacterEntity* CreateCharacterEntity(const FVector& Location = FVector::ZeroVector, const FRotator& Rotation = FRotator::ZeroRotator);

	UMWCharacterEntity* SpawnCharacter(const FVector& Location = FVector::ZeroVector, const FRotator& Rotation = FRotator::ZeroRotator);

	UMWCharacterEntity* FindCharacter(const FObjectId& EntityId);

	void DespawnCharacter(const FObjectId& EntityId);

private:
	void RemoveCharacter(const FObjectId& EntityId);

private:
	UPROPERTY()
	TMap<FObjectId, UMWCharacterEntity*> CharacterMap;
};