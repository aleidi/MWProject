#pragma once

// Include Header
#include "Entity/MWEntity.h"
#include "MWCharacterEntity.generated.h"

// Forward Declare
class AMWCharacter;
struct FMWCharacterResourceData;

// Define

/*
 * @class UMWCharacterEntity
 * 
 * @brief
 *
 * @note
 */
UCLASS()
class UMWCharacterEntity : public UMWEntity
{
	GENERATED_BODY()

public:
	virtual void Initialize() override;
	virtual void Uninitialize() override;	
	virtual UClass* GetActorClass() const override;
	virtual void OnActorLoaded(AActor* InLoadedActor) override;
	virtual FVector GetLocation() const override;

	void SetCharacterData(const FMWCharacterResourceData* InData);

	AMWCharacter* GetCharacterActor();
	const AMWCharacter* GetCharacterActor() const;

protected:
	const FMWCharacterResourceData* ResourceDataPtr;

	TSubclassOf<AMWCharacter> CharacterClass;
};